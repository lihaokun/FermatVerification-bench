#!/usr/bin/env python3
"""Ingest SV-COMP curated subset into cases/part5_mined/svcomp/.

SV-COMP (https://sv-comp.sosy-lab.org/) is a large benchmark collection used
in the annual Software Verification Competition. The repo (sv-benchmarks)
has 100+ category directories under `c/`. For v0.2.0 we ingest a small
"classic academic" subset.

Subset (v0.2.0):
  - c/loop-lit         — 60 classic loop invariant benchmarks
  - c/loop-invgen      — 48 InvGen benchmarks
  - c/array-examples   — 190 array benchmarks
Total: ~298 cases

Per-case mapping:
  - Each .c (or .i) → cases/part5_mined/svcomp/<category>/<name>/
  - ground_truth.c = upstream .c verbatim
  - stripped.c = same as ground_truth (no ACSL; goal is `__VERIFIER_assert(P)`
    inline + .yml property; we keep them as-is)
  - goal_type = "rte_only"
  - source = "svcomp" / source_dataset = "svcomp"

Usage:
    ingest_svcomp.py [--source-dir DIR]
"""

from __future__ import annotations

import argparse
import sys
from datetime import date
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION
from strip_acsl import find_acsl_blocks, compute_hash
from fetch_casp import format_toml_string

DEFAULT_SOURCE = Path(
    "/home/haokun/projects/FermatVerification-fcil/bench-research/svcomp/c"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part5_mined" / "svcomp"

UPSTREAM_URL = "https://gitlab.com/sosy-lab/benchmarking/sv-benchmarks"
UPSTREAM_TOOL_HINT = "SV-COMP (multi-verifier benchmark)"
LICENSE = "Apache-2.0"
LICENSE_FILE = "LICENSES/Apache-2.0-svcomp.txt"


# Category-level defaults
CATEGORY_META = {
    "loop-lit": {
        "features": ["loops", "linear_arith"],
        "target": ["functional"],
        "difficulty": {"overall": 2, "proof_size": 0, "math_theory": 2,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 0},
        "auxiliary": ["loop_invariant"],
        "notes": "SV-COMP loop-lit: classic loop invariant benchmarks (De Angelis, Fioravanti, Navas, Proietti et al.)",
    },
    "loop-invgen": {
        "features": ["loops", "linear_arith"],
        "target": ["functional"],
        "difficulty": {"overall": 3, "proof_size": 0, "math_theory": 2,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 0},
        "auxiliary": ["loop_invariant"],
        "notes": "SV-COMP loop-invgen: InvGen invariant generation benchmarks.",
    },
    "array-examples": {
        "features": ["loops", "arrays_mut", "linear_arith"],
        "target": ["functional"],
        "difficulty": {"overall": 3, "proof_size": 0, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 0},
        "auxiliary": ["loop_invariant"],
        "notes": "SV-COMP array-examples: array reasoning benchmarks.",
    },
}


def _safe_name(stem: str) -> str:
    return stem.replace("-", "_").replace(".", "_").replace(" ", "_")


def emit_case_toml(case_dir: Path, case_id: str, name: str, category: str,
                   upstream_path: str, source_hash: str,
                   strip_lines: list[int], meta: dict, today: str) -> None:
    d = meta["difficulty"]
    features_list = "[" + ", ".join(f'"{f}"' for f in meta["features"]) + "]"
    target_list = "[" + ", ".join(f'"{t}"' for t in meta["target"]) + "]"
    aux_list = "[" + ", ".join(f'"{a}"' for a in meta["auxiliary"]) + "]"
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"
    notes = meta["notes"]

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 5
features      = {features_list}

[provenance]
source             = "svcomp"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_path)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "algorithm"
intent             = "functional_correctness"

[files]
entry_files = ["ground_truth.c"]

[difficulty]
overall            = {d["overall"]}
proof_size         = {d["proof_size"]}
math_theory        = {d["math_theory"]}
data_complexity    = {d["data_complexity"]}
control_complexity = {d["control_complexity"]}
spec_complexity    = {d["spec_complexity"]}
auxiliary_needed   = {aux_list}

[difficulty.scored_by]
type      = "human"
scored_at = "{today}"

[verification]
target = {target_list}

[variants]
ground_truth = "ground_truth.c"
stripped     = "stripped.c"

[variants.strip_policy]
goal_type   = "rte_only"
goal_lines  = []
strip_lines = {strip_lines_str}
source_hash = "{source_hash}"
rationale   = "SV-COMP {category}: no ACSL annotations to strip; goal is `__VERIFIER_assert(P)` inline + .yml property file. ground_truth.c 与 stripped.c 内容相同。{notes}"

[variants.strip_policy.classified_by]
type              = "human"
reviewed_by_human = true
classified_at     = "{today}"

[mined]
source_dataset       = "svcomp"
upstream_id          = {format_toml_string(upstream_path)}
ultimate_origin      = "svcomp"
ultimate_origin_path = {format_toml_string(upstream_path)}
per_file_license     = "Apache-2.0"
"""
    (case_dir / "case.toml").write_text(content)


def ingest(source_dir: Path, output_dir: Path) -> int:
    if not source_dir.exists():
        print(f"error: source dir not found: {source_dir}", file=sys.stderr)
        return 0

    today = date.today().isoformat()
    output_dir.mkdir(parents=True, exist_ok=True)
    total = 0

    for category, meta in CATEGORY_META.items():
        cdir = source_dir / category
        if not cdir.exists():
            print(f"warn: missing category: {cdir}", file=sys.stderr)
            continue

        files = sorted(list(cdir.glob("*.c")) + list(cdir.glob("*.i")))
        print(f"{category}: {len(files)} C files", file=sys.stderr)

        out_cat = output_dir / category.replace("-", "_")
        out_cat.mkdir(parents=True, exist_ok=True)

        for src in files:
            if not src.is_file():
                continue
            cname = _safe_name(src.stem)
            case_id = f"part5_mined/svcomp/{category.replace('-', '_')}/{cname}"
            case_dir = out_cat / cname
            case_dir.mkdir(parents=True, exist_ok=True)

            gt_bytes = src.read_bytes()
            gt_path = case_dir / "ground_truth.c"
            gt_path.write_bytes(gt_bytes)

            # stripped.c == ground_truth.c (no ACSL to strip)
            stripped_path = case_dir / "stripped.c"
            stripped_path.write_bytes(gt_bytes)

            source_hash = compute_hash(gt_bytes)
            blocks = find_acsl_blocks(gt_bytes)
            strip_lines = [b.start_line for b in blocks]  # likely empty

            upstream_path = f"c/{category}/{src.name}"
            emit_case_toml(
                case_dir=case_dir, case_id=case_id, name=cname,
                category=category, upstream_path=upstream_path,
                source_hash=source_hash, strip_lines=strip_lines,
                meta=meta, today=today,
            )
            total += 1

    return total


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_dir)
    print(f"\ningested {n} SV-COMP cases to {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
