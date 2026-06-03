#!/usr/bin/env python3
"""Ingest the Live-FM-Bench dataset from HuggingFace into Part 5.

See docs/architecture.md §7 and docs/manifest-schema.md §13.4 for design.

Live-FM-Bench (`fm-universe/Live-FM-Bench`) is a contamination-free, held-out
evaluation benchmark for LLM program verification (Code2Proof task). Each row is
a C program plus the properties to be verified (`//@ assert(...)` goals); the
gold ACSL solution is NOT shipped (the `output` column is empty by design).

So these are **task-only** cases: we emit `stripped.c` (= upstream `input`) and
NO `ground_truth.c`. The bench schema supports this (stripped required,
ground_truth optional — manifest-schema.md §10). source_hash binds to stripped.c
content; the §16.4 consistency check skips when ground_truth is absent.

The dataset is Apache-2.0 and small (~262KB), so the .c files are committed
directly to the repo (unlike CASP, which is fetched + gitignored). This script
reads the upstream `live-fm-bench.jsonl` (stdlib json — no pyarrow needed),
caches it under `tools/.cache/live_fm_bench/`, then expands each of the 360 rows
into:

    cases/part5_mined/live_fm_bench/<folder>/
        case.toml
        stripped.c

Usage:
    ingest_live_fm_bench.py [--limit N] [--refresh] [--output-dir DIR]

Convention (per manifest-schema.md §13.4):
    - source = "live_fm_bench"
    - license = "Apache-2.0"
    - quality = "algorithm"
    - intent = "functional_correctness"
    - goal_type = "explicit_assert"  (the //@ assert goals to prove)
    - classified_by.type / scored_by.type = "upstream_split" (difficulty all-zero;
      run classify_case.py later to fill via LLM)
"""

from __future__ import annotations

import argparse
import json
import sys
import urllib.request
from pathlib import Path

# Adjust sys.path so we can import sibling modules
sys.path.insert(0, str(Path(__file__).resolve().parent))

from schema_version import SCHEMA_VERSION
from strip_acsl import compute_hash


LFMB_JSONL_URL = (
    "https://huggingface.co/datasets/fm-universe/Live-FM-Bench/"
    "resolve/main/live-fm-bench.jsonl"
)
LFMB_HF_URL = "https://huggingface.co/datasets/fm-universe/Live-FM-Bench"

REPO_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CACHE = REPO_ROOT / "tools" / ".cache" / "live_fm_bench"
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part5_mined" / "live_fm_bench"


def download_jsonl(cache_dir: Path, refresh: bool) -> Path:
    cache_dir.mkdir(parents=True, exist_ok=True)
    target = cache_dir / "live-fm-bench.jsonl"
    if target.exists() and not refresh:
        print(f"using cached jsonl: {target}", file=sys.stderr)
        return target
    print(f"downloading Live-FM-Bench jsonl from {LFMB_JSONL_URL}", file=sys.stderr)
    req = urllib.request.Request(LFMB_JSONL_URL, headers={"User-Agent": "curl/8"})
    with urllib.request.urlopen(req) as resp:
        target.write_bytes(resp.read())
    print(f"saved: {target} ({target.stat().st_size} bytes)", file=sys.stderr)
    return target


def format_toml_string(s: str) -> str:
    r"""Format a Python string as a TOML basic string literal.

    Escapes special characters per TOML spec: \, ", control chars.
    (Same helper as fetch_casp.py.)
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
    """Make one path segment filesystem-safe and consistent with svcomp dirs.

    Dashes/spaces/dots -> underscores (existing svcomp dirs use underscores, e.g.
    `loop_lit`, `array_examples`). Any other non-[A-Za-z0-9_] char -> underscore.
    """
    out = []
    for ch in seg:
        if ch.isalnum() or ch == "_":
            out.append(ch)
        else:
            out.append("_")
    return "".join(out)


def folder_to_relpath(filefolder_name: str) -> str:
    """Map upstream `filefolder_name` (e.g. 'array-tiling/rew') to a relative dir
    path under the output root. Each '/'-segment is sanitized independently;
    filefolder_name is globally unique upstream, so the result is collision-free.
    """
    segs = [sanitize_segment(s) for s in filefolder_name.split("/") if s]
    return "/".join(segs)


def find_assert_lines(content: bytes) -> list[int]:
    """1-indexed line numbers of ACSL assert goals (`//@ assert` / `/*@ assert`).

    These are the verification goals the consumer must discharge; recorded as
    `goal_lines` in strip_policy for informational use.
    """
    lines = content.split(b"\n")
    out = []
    for i, raw in enumerate(lines, start=1):
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
    goal_lines: list[int],
    classified_at: str,
) -> None:
    """Render case.toml per manifest-schema.md §13.4 (Live-FM-Bench task-only)."""
    goal_lines_str = "[" + ", ".join(str(x) for x in goal_lines) + "]"
    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 5
features      = []

[provenance]
source             = "live_fm_bench"
upstream_url       = "{LFMB_HF_URL}"
upstream_path      = {format_toml_string(filefolder_name)}
upstream_tool_hint = "frama-c/WP (Code2Proof spec-generation target; gold solution held out)"
license            = "Apache-2.0"
license_file       = "LICENSES/Apache-2.0-live-fm-bench.txt"
quality            = "algorithm"
intent             = "functional_correctness"

[files]
entry_files = ["stripped.c"]

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
scored_at = "{classified_at}"

[verification]
target = ["functional"]

[variants]
stripped = "stripped.c"

[variants.strip_policy]
goal_type   = "explicit_assert"
goal_lines  = {goal_lines_str}
strip_lines = []
source_hash = "{source_hash}"
rationale   = "Live-FM-Bench task-only: held-out 评测集，上游 output 列为空（gold ACSL 解不下发）。stripped.c = 上游 input（C 程序 + //@ assert 待验证性质），无 ground_truth。source_hash 绑定 stripped 内容。"

[variants.strip_policy.classified_by]
type              = "upstream_split"
reviewed_by_human = false
classified_at     = "{classified_at}"

[mined]
source_dataset       = "live_fm_bench"
upstream_id          = {format_toml_string(filefolder_name)}
ultimate_origin      = "svcomp"
ultimate_origin_path = {format_toml_string(filefolder_name)}
per_file_license     = "Apache-2.0"

[mined.live_fm_bench]
task           = "Code2Proof"
folder         = {format_toml_string(filefolder_name)}
file_name      = {format_toml_string(file_name)}
dataset_source = {format_toml_string(dataset_source)}
"""
    (case_dir / "case.toml").write_text(content)


def ingest(jsonl_path: Path, output_dir: Path, limit: int | None) -> int:
    rows = []
    for ln in jsonl_path.read_text(encoding="utf-8").splitlines():
        ln = ln.strip()
        if ln:
            rows.append(json.loads(ln))

    if limit is not None:
        rows = rows[:limit]

    from datetime import date
    classified_at = date.today().isoformat()

    output_dir.mkdir(parents=True, exist_ok=True)
    seen_dirs: set[str] = set()
    emitted = 0

    for idx, row in enumerate(rows, start=1):
        filefolder_name = row.get("filefolder_name") or ""
        file_name = row.get("file_name") or ""
        input_text = row.get("input")
        dataset_source = row.get("source") or ""

        if not isinstance(input_text, str) or not input_text:
            print(f"warn: row {idx} ({filefolder_name!r}) has empty input, skipping",
                  file=sys.stderr)
            continue

        relpath = folder_to_relpath(filefolder_name) or f"{idx:05d}"
        if relpath in seen_dirs:
            # filefolder_name is unique upstream; guard against post-sanitize
            # collisions just in case.
            relpath = f"{relpath}__{idx:05d}"
        seen_dirs.add(relpath)

        case_dir = output_dir / relpath
        case_dir.mkdir(parents=True, exist_ok=True)
        case_id = f"part5_mined/live_fm_bench/{relpath}"

        stripped_bytes = input_text.encode("utf-8")
        (case_dir / "stripped.c").write_bytes(stripped_bytes)

        source_hash = compute_hash(stripped_bytes)
        goal_lines = find_assert_lines(stripped_bytes)

        name = file_name or relpath.split("/")[-1]

        emit_case_toml(
            case_dir=case_dir,
            case_id=case_id,
            name=name,
            filefolder_name=filefolder_name,
            file_name=file_name,
            dataset_source=dataset_source,
            source_hash=source_hash,
            goal_lines=goal_lines,
            classified_at=classified_at,
        )

        print(f"  case {case_id}: {len(goal_lines)} assert goal(s), "
              f"{len(stripped_bytes)} bytes", file=sys.stderr)
        emitted += 1

    return emitted


def main() -> None:
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

    jsonl_path = download_jsonl(args.cache_dir, args.refresh)
    n = ingest(jsonl_path, args.output_dir, args.limit)
    print(f"\nemitted {n} cases under {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
