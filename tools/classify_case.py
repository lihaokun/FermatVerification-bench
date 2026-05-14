#!/usr/bin/env python3
"""LLM-driven classifier for case.toml's [variants.strip_policy] + [difficulty].

See docs/classify-prompts.md for prompt design + version policy. The current
prompt is v0.1 (focuses on difficulty scoring + goal_type detection).

Usage:
    classify_case.py --case <case-id>          # single case
    classify_case.py --batch <glob>            # batch (e.g. "part5_mined/casp/*")
    classify_case.py --dry-run --case <id>     # print prompt, don't call LLM
    classify_case.py --apply --case <id>       # write result into case.toml (v0.2+)

Default behavior (no --apply): writes `.classify-suggested.json` next to
case.toml. User can review and apply manually.

Environment:
    ANTHROPIC_API_KEY  — required for non-dry-run mode

Cost note: 1933 cases × ~1-2K tokens/case ≈ $20-50 with claude-opus-4-7.
Start with --dry-run + small batches to validate prompt quality.
"""

from __future__ import annotations

import argparse
import json
import os
import sys
import tomllib
from dataclasses import dataclass
from pathlib import Path
from typing import Any

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION
from lib import vocab

PROMPT_VERSION = "v0.1"
DEFAULT_MODEL = "claude-opus-4-7"
DEFAULT_TEMPERATURE = 0.0
MAX_GROUND_TRUTH_BYTES = 100_000  # ~25K tokens worth; beyond this we truncate

SYSTEM_PROMPT = """You are an expert in C program verification with ACSL (ANSI/ISO C Specification
Language). Your task is to analyze a C+ACSL source file from a verification
benchmark and produce structured metadata about it.

You will receive:
  - The full source content (C code + ACSL annotations)
  - Some context (part number, declared features, declared verification target)

You output a single JSON object with the schema specified by the user. Do not
output any explanation outside the JSON. Use the user's exact field names.

Be calibrated: most cases are 1-3 in difficulty; reserve 4-5 for genuinely
complex cases (induction, custom theories, multi-file projects, 100+ ACSL
annotations)."""


def load_case_toml(case_dir: Path) -> dict:
    return tomllib.loads((case_dir / "case.toml").read_text())


def render_user_prompt(case_data: dict, ground_truth_path: Path) -> str:
    content = ground_truth_path.read_bytes()
    content_text = content.decode("utf-8", errors="replace")

    truncated_note = ""
    if len(content) > MAX_GROUND_TRUTH_BYTES:
        head = MAX_GROUND_TRUTH_BYTES // 2
        tail = MAX_GROUND_TRUTH_BYTES // 2
        content_text = (
            content_text[:head]
            + f"\n\n[... truncated {len(content) - MAX_GROUND_TRUTH_BYTES} bytes ...]\n\n"
            + content_text[-tail:]
        )
        truncated_note = " (TRUNCATED for prompt; original was larger)"

    features = case_data.get("features", [])
    target = case_data.get("verification", {}).get("target", [])
    part = case_data.get("part", 0)
    source = case_data.get("provenance", {}).get("source", "")
    byte_count = len(content)
    line_count = content.count(b"\n") + 1

    template = f"""Analyze this C+ACSL case from the FermatVerification-bench. Output JSON with
fields below.

CASE CONTEXT:
  part:     {part}
  features: {features}
  target:   {target}
  source:   {source}
  file size: {byte_count} bytes / {line_count} lines{truncated_note}

SOURCE CONTENT (ground_truth):
```
{content_text}
```

REQUIRED OUTPUT JSON:

{{
  "goal_type": "rte_only" | "explicit_assert" | "explicit_lemma" | "mixed",
  "goal_lines": [<line numbers of ACSL blocks that ARE the verification goal>],
  "rationale": "<1-3 sentence explanation>",
  "difficulty": {{
    "overall": 0-5,
    "proof_size": 0-5,
    "math_theory": 0-5,
    "data_complexity": 0-5,
    "control_complexity": 0-5,
    "spec_complexity": 0-5
  }},
  "auxiliary_needed": [<one or more of: loop_invariant, predicate_def, lemma, ghost, induction>]
}}

DEFINITIONS:

- goal_type:
  * rte_only       — no explicit ACSL goal; objective is "WP+RTE全过"
  * explicit_assert — top-level/in-function `assert` is the final goal
  * explicit_lemma  — explicit `lemma` clause is the final conclusion
  * mixed           — mixture of the above

  Function pre/post (requires/ensures/assigns) is PROCESS spec, NOT goal.
  Goal is the ULTIMATE target. Only obviously non-scaffolding assert/lemma counts.

- goal_lines: starting line numbers (1-indexed) of each goal-bearing ACSL block.
  rte_only typically returns [].

- difficulty 0-5 anchors:
  * proof_size: ACSL block count (0=0-2 / 1=3-10 / 2=11-30 / 3=31-100 / 4=101-300 / 5=>300)
  * math_theory: 0=boolean / 1=Presburger / 2=linear+array / 3=nonlinear/bv / 4=mixed / 5=custom
  * data_complexity: 0=pure / 1=scalar / 2=array / 3=pointer/struct / 4=linked/tree / 5=composite heap
  * control_complexity: 0=straight / 1=branch / 2=single loop / 3=nested / 4=recursion / 5=mutual
  * spec_complexity: 0=none / 1=pre/post / 2=behaviors / 3=ghost/axiomatic / 4=lemma / 5=induction
  * overall: integrated (not formulaic; judge independently)

- auxiliary_needed: subset of {{loop_invariant, predicate_def, lemma, ghost, induction}}.

OUTPUT JSON ONLY — no surrounding text, no markdown fences."""
    return template


@dataclass
class ClassifyResult:
    goal_type: str
    goal_lines: list[int]
    rationale: str
    difficulty: dict[str, int]
    auxiliary_needed: list[str]


def validate_llm_output(payload: dict) -> tuple[ClassifyResult | None, list[str]]:
    """Return (result, errors). result=None iff any error."""
    errs = []

    gt = payload.get("goal_type")
    if gt not in vocab.GOAL_TYPE:
        errs.append(f"goal_type {gt!r} not in vocabulary")

    gl = payload.get("goal_lines")
    if not isinstance(gl, list) or not all(isinstance(x, int) for x in gl):
        errs.append("goal_lines must be list[int]")

    rat = payload.get("rationale")
    if not isinstance(rat, str) or not rat.strip():
        errs.append("rationale must be non-empty string")

    diff = payload.get("difficulty", {})
    if not isinstance(diff, dict):
        errs.append("difficulty must be a dict")
    else:
        for k in ("overall", "proof_size", "math_theory",
                  "data_complexity", "control_complexity", "spec_complexity"):
            v = diff.get(k)
            if not isinstance(v, int) or not (0 <= v <= 5):
                errs.append(f"difficulty.{k} must be int 0-5, got {v!r}")

    aux = payload.get("auxiliary_needed")
    if not isinstance(aux, list):
        errs.append("auxiliary_needed must be list")
    else:
        for x in aux:
            if x not in vocab.AUXILIARY:
                errs.append(f"auxiliary_needed[{x!r}] not in vocabulary")

    if errs:
        return None, errs
    return ClassifyResult(
        goal_type=gt,
        goal_lines=gl,
        rationale=rat,
        difficulty=diff,
        auxiliary_needed=aux,
    ), []


def call_llm(system_prompt: str, user_prompt: str, model: str,
             temperature: float) -> str:
    """Call Anthropic API. Raises if SDK not installed or API key missing."""
    try:
        from anthropic import Anthropic
    except ImportError:
        raise RuntimeError(
            "anthropic SDK not installed. Install with:\n"
            "  .venv/bin/pip install anthropic\n"
            "Or use --dry-run to just render prompts without calling."
        )

    api_key = os.environ.get("ANTHROPIC_API_KEY")
    if not api_key:
        raise RuntimeError(
            "ANTHROPIC_API_KEY env var not set. Either:\n"
            "  export ANTHROPIC_API_KEY=...\n"
            "Or use --dry-run."
        )

    client = Anthropic(api_key=api_key)
    response = client.messages.create(
        model=model,
        max_tokens=2000,
        temperature=temperature,
        system=system_prompt,
        messages=[{"role": "user", "content": user_prompt}],
    )
    # Extract text content
    parts = [b.text for b in response.content if hasattr(b, "text")]
    return "".join(parts)


def classify_one(case_dir: Path, model: str, temperature: float,
                 dry_run: bool, apply_to_toml: bool) -> bool:
    """Classify one case. Return True on success."""
    rel = case_dir.relative_to(REPO_ROOT / "cases").as_posix()
    print(f"\n=== {rel} ===", file=sys.stderr)

    case_data = load_case_toml(case_dir)
    part = case_data.get("part", 0)
    if part not in (3, 4, 5):
        print(f"  skip: part={part} (classify only Part 3/4/5)", file=sys.stderr)
        return True

    var = case_data.get("variants", {})
    gt_rel = var.get("ground_truth")
    if not gt_rel:
        print(f"  skip: no ground_truth", file=sys.stderr)
        return True

    gt_path = case_dir / gt_rel
    if not gt_path.exists():
        print(f"  skip: ground_truth file missing ({gt_path})", file=sys.stderr)
        return True

    user_prompt = render_user_prompt(case_data, gt_path)

    if dry_run:
        print(f"  prompt: {len(user_prompt)} chars (dry-run; not calling LLM)",
              file=sys.stderr)
        suggested_path = case_dir / ".classify-prompt-dryrun.txt"
        suggested_path.write_text(
            f"=== SYSTEM ===\n{SYSTEM_PROMPT}\n\n=== USER ===\n{user_prompt}\n"
        )
        print(f"  wrote: {suggested_path.relative_to(REPO_ROOT)}", file=sys.stderr)
        return True

    # Real LLM call
    try:
        raw = call_llm(SYSTEM_PROMPT, user_prompt, model, temperature)
    except RuntimeError as e:
        print(f"  error: {e}", file=sys.stderr)
        return False

    # Parse JSON
    try:
        payload = json.loads(raw.strip())
    except json.JSONDecodeError as e:
        print(f"  error: LLM output not valid JSON: {e}", file=sys.stderr)
        print(f"  raw output: {raw[:500]}...", file=sys.stderr)
        return False

    result, errs = validate_llm_output(payload)
    if errs:
        print(f"  error: LLM output schema invalid:", file=sys.stderr)
        for e in errs:
            print(f"    - {e}", file=sys.stderr)
        return False

    # Write suggested file (default; --apply not implemented v0.1)
    suggested = {
        "$schema_note": "classify_case.py output; see docs/classify-prompts.md",
        "model": model,
        "prompt_version": PROMPT_VERSION,
        "case_id": case_data.get("id", ""),
        "result": {
            "goal_type": result.goal_type,
            "goal_lines": result.goal_lines,
            "rationale": result.rationale,
            "difficulty": result.difficulty,
            "auxiliary_needed": result.auxiliary_needed,
        },
    }
    out_path = case_dir / ".classify-suggested.json"
    out_path.write_text(json.dumps(suggested, indent=2, ensure_ascii=False))
    print(f"  wrote: {out_path.relative_to(REPO_ROOT)}", file=sys.stderr)

    if apply_to_toml:
        print(f"  TODO: --apply not yet implemented (v0.2.x)", file=sys.stderr)

    return True


def find_cases(filter_path: str | None) -> list[Path]:
    cases_dir = REPO_ROOT / "cases"
    all_cases = sorted(p.parent for p in cases_dir.rglob("case.toml"))
    if filter_path is None:
        return all_cases
    matched = []
    for c in all_cases:
        rel = c.relative_to(cases_dir).as_posix()
        if rel.startswith(filter_path) or filter_path in rel:
            matched.append(c)
    return matched


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--case", help="Single case id (relative to cases/), e.g. part5_mined/casp/00001")
    g.add_argument("--batch", help="Batch glob/prefix, e.g. part5_mined/casp or part3_curated")
    p.add_argument("--dry-run", action="store_true",
                   help="Render prompt only, don't call LLM (no API key needed)")
    p.add_argument("--apply", action="store_true",
                   help="(v0.2.x) Apply result to case.toml; currently writes .classify-suggested.json")
    p.add_argument("--model", default=DEFAULT_MODEL)
    p.add_argument("--temperature", type=float, default=DEFAULT_TEMPERATURE)
    p.add_argument("--limit", type=int, default=None,
                   help="Limit batch to first N cases")
    args = p.parse_args()

    if args.case:
        case_dir = REPO_ROOT / "cases" / args.case
        if not (case_dir / "case.toml").exists():
            print(f"error: case not found: {case_dir}", file=sys.stderr)
            sys.exit(1)
        ok = classify_one(case_dir, args.model, args.temperature,
                          args.dry_run, args.apply)
        sys.exit(0 if ok else 1)

    # batch
    cases = find_cases(args.batch)
    if args.limit:
        cases = cases[: args.limit]
    print(f"batch: {len(cases)} cases matched", file=sys.stderr)

    n_ok = 0
    n_err = 0
    for c in cases:
        if classify_one(c, args.model, args.temperature, args.dry_run, args.apply):
            n_ok += 1
        else:
            n_err += 1
    print(f"\nbatch done: {n_ok} ok, {n_err} errors", file=sys.stderr)
    sys.exit(0 if n_err == 0 else 1)


if __name__ == "__main__":
    main()
