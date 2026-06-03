#!/usr/bin/env python3
"""Ingest the FM-Bench-Verified dataset from HuggingFace into Part 5.

See docs/architecture.md §7 and docs/manifest-schema.md §13.3 for design.

FM-Bench-Verified (`fm-universe/FM-bench-verified`) is a manually-cleaned and
WP-verified benchmark for LLM program verification (Code2Proof task). All 280
cases are verified through Frama-C's WP. Unlike the held-out Live-FM-Bench, the
gold ACSL solution IS shipped (the `output` column is populated), so these are
**double-version** cases (like CASP):

    ground_truth.c = upstream `output`  (verified C + ACSL, passes Frama-C WP)
    stripped.c     = upstream `input`   (C program + `//@ assert` goals, specs removed)

source_hash binds to ground_truth.c; the §16.4 consistency check validates it
(`validate_manifest.py --check-strip-hash`).

The dataset is Apache-2.0 and small (~483KB), so the .c files are committed
directly to the repo (like Live-FM-Bench, unlike CASP). This script reads the
upstream `fm-bench-verified.jsonl` (stdlib json — no pyarrow needed), caches it
under `tools/.cache/fm_bench_verified/`, then expands each of the 280 rows into:

    cases/part5_mined/fm_bench_verified/<source>/<folder>/
        case.toml
        ground_truth.c
        stripped.c

Usage:
    ingest_fm_bench_verified.py [--limit N] [--refresh] [--output-dir DIR]

Convention (per manifest-schema.md §13.3):
    - source = "fm_bench_verified"
    - license = "Apache-2.0"
    - quality = "algorithm"
    - intent = "functional_correctness"
    - goal_type = per-case from ground_truth (explicit_assert / mixed / ...)
    - classified_by.type / scored_by.type = "upstream_split" (difficulty all-zero;
      run classify_case.py later to fill via LLM)
"""

from __future__ import annotations

import argparse
import json
import re
import sys
import urllib.request
from pathlib import Path

# Adjust sys.path so we can import sibling modules
sys.path.insert(0, str(Path(__file__).resolve().parent))

from schema_version import SCHEMA_VERSION
from strip_acsl import compute_hash, find_acsl_blocks


FMV_JSONL_URL = (
    "https://huggingface.co/datasets/fm-universe/FM-bench-verified/"
    "resolve/main/fm-bench-verified.jsonl"
)
FMV_HF_URL = "https://huggingface.co/datasets/fm-universe/FM-bench-verified"

REPO_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CACHE = REPO_ROOT / "tools" / ".cache" / "fm_bench_verified"
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part5_mined" / "fm_bench_verified"

ASSERT_RE = re.compile(r"(//@|/\*@)\s*assert")


def download_jsonl(cache_dir: Path, refresh: bool) -> Path:
    cache_dir.mkdir(parents=True, exist_ok=True)
    target = cache_dir / "fm-bench-verified.jsonl"
    if target.exists() and not refresh:
        print(f"using cached jsonl: {target}", file=sys.stderr)
        return target
    print(f"downloading FM-Bench-Verified jsonl from {FMV_JSONL_URL}", file=sys.stderr)
    req = urllib.request.Request(FMV_JSONL_URL, headers={"User-Agent": "curl/8"})
    with urllib.request.urlopen(req) as resp:
        target.write_bytes(resp.read())
    print(f"saved: {target} ({target.stat().st_size} bytes)", file=sys.stderr)
    return target


def format_toml_string(s: str) -> str:
    r"""Format a Python string as a TOML basic string literal.

    Escapes special characters per TOML spec: \, ", control chars.
    (Same helper as fetch_casp.py / ingest_live_fm_bench.py.)
    """
    out = []
    for ch in s:
        cp = ord(ch)
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif cp < 0x20:
            if ch == "\b":
                out.append("\\b")
            elif ch == "\t":
                out.append("\\t")
            elif ch == "\n":
                out.append("\\n")
            elif ch == "\f":
                out.append("\\f")
            elif ch == "\r":
                out.append("\\r")
            else:
                out.append(f"\\u{cp:04x}")
        else:
            out.append(ch)
    return '"' + "".join(out) + '"'


def sanitize_segment(seg: str) -> str:
    """Make one path segment filesystem-safe (non-[A-Za-z0-9_] -> underscore)."""
    return "".join(ch if (ch.isalnum() or ch == "_") else "_" for ch in seg)


def classify_goal_type(ground_truth: str) -> str:
    """Pick goal_type from the verified ground_truth content (§15.9)."""
    has_assert = bool(ASSERT_RE.search(ground_truth))
    has_lemma = "lemma " in ground_truth
    if has_assert and has_lemma:
        return "mixed"
    if has_assert:
        return "explicit_assert"
    if has_lemma:
        return "explicit_lemma"
    return "rte_only"


def find_assert_lines(content: bytes) -> list[int]:
    """1-indexed line numbers of ACSL assert goals (`//@ assert` / `/*@ assert`)."""
    out = []
    for i, raw in enumerate(content.split(b"\n"), start=1):
        s = raw.strip()
        if (s.startswith(b"//@") or s.startswith(b"/*@")) and b"assert" in s:
            out.append(i)
    return out


def emit_case_toml(
    case_dir: Path,
    case_id: str,
    name: str,
    filefolder_name: str,
    file_name: str,
    dataset_source: str,
    source_hash: str,
    goal_type: str,
    goal_lines: list[int],
    strip_lines: list[int],
) -> None:
    """Render case.toml per manifest-schema.md §13.3 (FM-Bench-Verified double-version)."""
    goal_lines_str = "[" + ", ".join(str(x) for x in goal_lines) + "]"
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"
    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 5
features      = []

[provenance]
source             = "fm_bench_verified"
upstream_url       = "{FMV_HF_URL}"
upstream_path      = {format_toml_string(filefolder_name)}
upstream_tool_hint = "frama-c/WP (all 280 cases verified through Frama-C WP)"
license            = "Apache-2.0"
license_file       = "LICENSES/Apache-2.0-fm-bench-verified.txt"
quality            = "algorithm"
intent             = "functional_correctness"

[files]
entry_files = ["ground_truth.c"]

[difficulty]
overall            = 0
proof_size         = 0
math_theory        = 0
data_complexity    = 0
control_complexity = 0
spec_complexity    = 0
auxiliary_needed   = []

[difficulty.scored_by]
type      = "upstream_split"
scored_at = "{CLASSIFIED_AT}"

[verification]
target = ["functional"]

[variants]
ground_truth = "ground_truth.c"
stripped     = "stripped.c"

[variants.strip_policy]
goal_type   = "{goal_type}"
goal_lines  = {goal_lines_str}
strip_lines = {strip_lines_str}
source_hash = "{source_hash}"
rationale   = "FM-Bench-Verified 上游双版本: ground_truth.c = 上游 output（人工清洗 + 经 Frama-C WP 验证的 C+ACSL），stripped.c = 上游 input（C 程序 + //@ assert 待验证性质，缺规约解）。strip_lines = ground_truth 中 ACSL 块起始行（信息性，同 CASP）。"

[variants.strip_policy.classified_by]
type              = "upstream_split"
reviewed_by_human = false
classified_at     = "{CLASSIFIED_AT}"

[mined]
source_dataset       = "fm_bench_verified"
upstream_id          = {format_toml_string(filefolder_name)}
ultimate_origin      = ""
ultimate_origin_path = {format_toml_string(filefolder_name)}
per_file_license     = "Apache-2.0"

[mined.fm_bench_verified]
task           = "Code2Proof"
folder         = {format_toml_string(filefolder_name)}
file_name      = {format_toml_string(file_name)}
dataset_source = {format_toml_string(dataset_source)}
"""
    (case_dir / "case.toml").write_text(content)


# Filled in main() before ingest (Date.today not callable at import; set once).
CLASSIFIED_AT = ""


def ingest(jsonl_path: Path, output_dir: Path, limit: int | None) -> int:
    rows = []
    for ln in jsonl_path.read_text(encoding="utf-8").splitlines():
        ln = ln.strip()
        if ln:
            rows.append(json.loads(ln))

    if limit is not None:
        rows = rows[:limit]

    output_dir.mkdir(parents=True, exist_ok=True)
    seen_dirs: set[str] = set()
    emitted = 0

    for idx, row in enumerate(rows, start=1):
        filefolder_name = row.get("filefolder_name") or ""
        file_name = row.get("file_name") or ""
        input_text = row.get("input")
        output_text = row.get("output")
        dataset_source = row.get("source") or ""

        if not isinstance(output_text, str) or not output_text:
            print(f"warn: row {idx} ({filefolder_name!r}) has empty output (ground_truth), skipping",
                  file=sys.stderr)
            continue
        if not isinstance(input_text, str) or not input_text:
            print(f"warn: row {idx} ({filefolder_name!r}) has empty input (stripped), skipping",
                  file=sys.stderr)
            continue

        relpath = f"{sanitize_segment(dataset_source)}/{sanitize_segment(filefolder_name)}"
        if relpath in seen_dirs:
            relpath = f"{relpath}__{idx:05d}"
        seen_dirs.add(relpath)

        case_dir = output_dir / relpath
        case_dir.mkdir(parents=True, exist_ok=True)
        case_id = f"part5_mined/fm_bench_verified/{relpath}"

        gt_bytes = output_text.encode("utf-8")
        stripped_bytes = input_text.encode("utf-8")
        (case_dir / "ground_truth.c").write_bytes(gt_bytes)
        (case_dir / "stripped.c").write_bytes(stripped_bytes)

        source_hash = compute_hash(gt_bytes)
        goal_type = classify_goal_type(output_text)
        goal_lines = find_assert_lines(gt_bytes)
        strip_lines = [b.start_line for b in find_acsl_blocks(gt_bytes)]

        name = file_name or sanitize_segment(filefolder_name)

        emit_case_toml(
            case_dir=case_dir,
            case_id=case_id,
            name=name,
            filefolder_name=filefolder_name,
            file_name=file_name,
            dataset_source=dataset_source,
            source_hash=source_hash,
            goal_type=goal_type,
            goal_lines=goal_lines,
            strip_lines=strip_lines,
        )

        print(f"  case {case_id}: goal_type={goal_type}, {len(strip_lines)} ACSL block(s), "
              f"gt={len(gt_bytes)}B stripped={len(stripped_bytes)}B", file=sys.stderr)
        emitted += 1

    return emitted


def main() -> None:
    global CLASSIFIED_AT
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--limit", type=int, default=None,
                   help="Only emit first N cases (POC mode)")
    p.add_argument("--refresh", action="store_true",
                   help="Re-download jsonl even if cached")
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT,
                   help=f"Output dir (default: {DEFAULT_OUTPUT})")
    p.add_argument("--cache-dir", type=Path, default=DEFAULT_CACHE,
                   help=f"jsonl cache dir (default: {DEFAULT_CACHE})")
    args = p.parse_args()

    from datetime import date
    CLASSIFIED_AT = date.today().isoformat()

    jsonl_path = download_jsonl(args.cache_dir, args.refresh)
    n = ingest(jsonl_path, args.output_dir, args.limit)
    print(f"\nemitted {n} cases under {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
