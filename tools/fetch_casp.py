#!/usr/bin/env python3
"""Fetch CASP dataset from HuggingFace and emit per-case bench files.

See docs/architecture.md §7.5 and docs/manifest-schema.md §13 for design.

CASP is NOT redistributed in this repo (per-file upstream licenses from The
Stack). This script downloads `train.parquet` on first run, caches it under
`tools/.cache/casp/`, then expands each of the 506 rows into:

    cases/part5_mined/casp/<id>/
        case.toml
        ground_truth.c
        stripped.c

Usage:
    fetch_casp.py [--limit N] [--refresh] [--output-dir DIR]

Behavior:
    - `--limit N`: only emit first N cases (POC use)
    - `--refresh`: re-download parquet even if cached
    - `--output-dir`: override default `cases/part5_mined/casp/`

Convention (per manifest-schema.md §13.2):
    - source = "casp"
    - license = "NOASSERTION" (per-file upstream license needs user lookup)
    - quality = "algorithm"
    - intent = "functional_correctness"
    - goal_type = "rte_only"
    - classified_by.type = "upstream_split"
    - difficulty.scored_by.type = "upstream_split" (defaults to all-zero;
      run classify_case.py --only difficulty later to fill via LLM)
"""

from __future__ import annotations

import argparse
import sys
import urllib.request
from pathlib import Path
from typing import TYPE_CHECKING

# Adjust sys.path so we can import sibling modules
sys.path.insert(0, str(Path(__file__).resolve().parent))

from schema_version import SCHEMA_VERSION
from strip_acsl import find_acsl_blocks, compute_hash

if TYPE_CHECKING:
    import pyarrow.parquet as pq  # noqa: F401


CASP_HF_PARQUET_URL = (
    "https://huggingface.co/datasets/nicher92/CASP_dataset/"
    "resolve/main/data/train-00000-of-00001.parquet"
)
CASP_HF_URL = "https://huggingface.co/datasets/nicher92/CASP_dataset"

REPO_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CACHE = REPO_ROOT / "tools" / ".cache" / "casp"
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part5_mined" / "casp"


def check_pyarrow() -> "pq":
    try:
        import pyarrow.parquet as pq_mod
        return pq_mod
    except ImportError:
        print(
            "error: pyarrow not installed. Set up a venv and install:\n"
            "  python3 -m venv .venv\n"
            "  .venv/bin/pip install pyarrow\n"
            "  .venv/bin/python tools/fetch_casp.py [...]",
            file=sys.stderr,
        )
        sys.exit(1)


def download_parquet(cache_dir: Path, refresh: bool) -> Path:
    cache_dir.mkdir(parents=True, exist_ok=True)
    target = cache_dir / "train.parquet"
    if target.exists() and not refresh:
        print(f"using cached parquet: {target}", file=sys.stderr)
        return target
    print(f"downloading CASP parquet from {CASP_HF_PARQUET_URL}", file=sys.stderr)
    with urllib.request.urlopen(CASP_HF_PARQUET_URL) as resp:
        target.write_bytes(resp.read())
    print(f"saved: {target} ({target.stat().st_size} bytes)", file=sys.stderr)
    return target


def format_toml_string(s: str) -> str:
    r"""Format a Python string as a TOML basic string literal.

    Escapes special characters per TOML spec: \, ", control chars.
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


def emit_case_toml(
    case_dir: Path,
    case_id: str,
    name: str,
    upstream_id: str,
    ground_truth_bytes: bytes,
    strip_lines: list[int],
    source_hash: str,
    upstream_verified: bool,
    total_goals: int,
    verified_goals: int,
    error_cause: str,
    classified_at: str,
) -> None:
    """Render case.toml per manifest-schema.md §13 (CASP variant)."""
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"
    error_cause_str = format_toml_string(error_cause)
    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 5
features      = []

[provenance]
source             = "casp"
upstream_url       = "{CASP_HF_URL}"
upstream_path      = {format_toml_string(upstream_id)}
upstream_tool_hint = "frama-c (version unknown; see CASP paper arXiv:2508.18798)"
license            = "NOASSERTION"
license_file       = "LICENSES/CASP-attribution.txt"
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
scored_at = "{classified_at}"

[verification]
target = ["functional"]

[variants]
ground_truth = "ground_truth.c"
stripped     = "stripped.c"

[variants.strip_policy]
goal_type   = "rte_only"
goal_lines  = []
strip_lines = {strip_lines_str}
source_hash = "{source_hash}"
rationale   = "CASP 上游 split: stripped = c_code_snippet, ground_truth = verified_c_file_content."

[variants.strip_policy.classified_by]
type              = "upstream_split"
reviewed_by_human = false
classified_at     = "{classified_at}"

[mined]
source_dataset       = "casp"
upstream_id          = {format_toml_string(upstream_id)}
ultimate_origin      = "the_stack_v2"
ultimate_origin_path = ""
per_file_license     = ""

[mined.casp]
upstream_verified = {str(upstream_verified).lower()}
total_goals       = {total_goals}
verified_goals    = {verified_goals}
error_cause       = {error_cause_str}
"""
    (case_dir / "case.toml").write_text(content)


def fetch(parquet_path: Path, output_dir: Path, limit: int | None) -> int:
    pq = check_pyarrow()
    table = pq.read_table(parquet_path)
    rows = table.to_pylist()

    if limit is not None:
        rows = rows[:limit]

    from datetime import date
    classified_at = date.today().isoformat()

    output_dir.mkdir(parents=True, exist_ok=True)

    for idx, row in enumerate(rows, start=1):
        case_id_suffix = f"{idx:05d}"
        case_id = f"part5_mined/casp/{case_id_suffix}"
        case_dir = output_dir / case_id_suffix
        case_dir.mkdir(parents=True, exist_ok=True)

        gt_text = row["verified_c_file_content"]
        if not isinstance(gt_text, str):
            print(f"warn: row {idx} has non-string verified_c_file_content, skipping", file=sys.stderr)
            continue
        stripped_text = row["c_code_snippet"] or ""

        gt_bytes = gt_text.encode("utf-8")
        stripped_bytes = stripped_text.encode("utf-8")

        gt_path = case_dir / "ground_truth.c"
        stripped_path = case_dir / "stripped.c"
        gt_path.write_bytes(gt_bytes)
        stripped_path.write_bytes(stripped_bytes)

        source_hash = compute_hash(gt_bytes)
        acsl_blocks = find_acsl_blocks(gt_bytes)
        strip_lines = [b.start_line for b in acsl_blocks]

        # name: try to extract first function name from C, fallback to id
        name = case_id_suffix

        emit_case_toml(
            case_dir=case_dir,
            case_id=case_id,
            name=name,
            upstream_id=row["file_name"] or "",
            ground_truth_bytes=gt_bytes,
            strip_lines=strip_lines,
            source_hash=source_hash,
            upstream_verified=bool(row["verified"]),
            total_goals=int(row["total_goals"]),
            verified_goals=int(row["verified_goals"]),
            error_cause=row["error_cause"] or "",
            classified_at=classified_at,
        )

        print(f"  case {case_id}: {len(acsl_blocks)} ACSL blocks, {len(gt_bytes)} bytes", file=sys.stderr)

    return len(rows)


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--limit", type=int, default=None,
                   help="Only emit first N cases (POC mode)")
    p.add_argument("--refresh", action="store_true",
                   help="Re-download parquet even if cached")
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT,
                   help=f"Output dir (default: {DEFAULT_OUTPUT})")
    p.add_argument("--cache-dir", type=Path, default=DEFAULT_CACHE,
                   help=f"Parquet cache dir (default: {DEFAULT_CACHE})")
    args = p.parse_args()

    parquet_path = download_parquet(args.cache_dir, args.refresh)
    n = fetch(parquet_path, args.output_dir, args.limit)
    print(f"\nemitted {n} cases under {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
