#!/usr/bin/env python3
"""Ingest Frama-C wp_gallery (13 用例) into cases/part3_curated/frama_c_wp_gallery/.

Per-case metadata is curated manually (see WP_GALLERY_METADATA below). All cases
use goal_type='rte_only': stripped version is pure C (zero ACSL); agent task
implicit task = "let WP+RTE discharge".

Usage:
    ingest_wp_gallery.py [--source-dir DIR]
    --source-dir defaults to frama-c 32 sources at /home/haokun/.opam/frama32/.../wp_gallery
"""

from __future__ import annotations

import argparse
import sys
from datetime import date
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION
from strip_acsl import find_acsl_blocks, compute_hash, strip
from fetch_casp import format_toml_string

DEFAULT_FRAMA_C_SOURCE = Path(
    "/home/haokun/.opam/frama32/.opam-switch/sources/frama-c.32.0/"
    "src/plugins/wp/tests/wp_gallery"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part3_curated" / "frama_c_wp_gallery"

UPSTREAM_URL = "https://git.frama-c.com/pub/frama-c"
UPSTREAM_TOOL_HINT = "frama-c 32 (Gallium)"
LICENSE = "LGPL-2.1-only"
LICENSE_FILE = "LICENSES/LGPL-2.1-frama-c.txt"


# Each entry: source filename → curated metadata
#
# features, difficulty, intent, target curated by reading each file.
# difficulty.scored_by.type = 'human' (initial manual curation during
# bootstrap; can be refined by classify_case.py later).
WP_GALLERY_METADATA: dict[str, dict] = {
    "binary-multiplication-without-overflow.c": {
        "name": "binary_multiplication_without_overflow",
        "features": ["loops", "bitwise", "overflow_concern", "linear_arith",
                     "lemma", "axiomatic"],
        "target": ["functional", "memory_safety"],
        "intent": "proof_technique",  # 演示 axiomatic + lemma + loop invariant 配合
        "difficulty": {"overall": 4, "proof_size": 1, "math_theory": 3,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 4},
        "auxiliary_needed": ["loop_invariant", "lemma"],
    },
    "binary-multiplication.c": {
        "name": "binary_multiplication",
        "features": ["loops", "bitwise", "linear_arith", "lemma", "axiomatic"],
        "target": ["functional"],
        "intent": "proof_technique",
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 3,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 4},
        "auxiliary_needed": ["loop_invariant", "lemma"],
    },
    "bsearch.c": {
        "name": "bsearch",
        "features": ["loops", "arrays_ro", "linear_arith"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 1},
        "auxiliary_needed": ["loop_invariant"],
    },
    "euclid.c": {
        "name": "euclid",
        "features": ["loops", "axiomatic", "logic_function", "termination"],
        "target": ["functional", "termination"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant"],
    },
    "euclid2.c": {
        "name": "euclid2",
        "features": ["loops", "termination", "logic_function"],
        "target": ["functional", "termination"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 0, "math_theory": 2,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary_needed": ["loop_invariant"],
    },
    "euclid3.c": {
        "name": "euclid3",
        "features": ["loops", "termination", "logic_function", "callers"],
        "target": ["functional", "termination"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary_needed": ["loop_invariant"],
    },
    "find.i": {
        "name": "find",
        "features": ["loops", "pointers", "arrays_ro", "predicate", "linear_arith"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 3, "control_complexity": 2, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "frama_c_exo1_solved.c": {
        "name": "frama_c_exo1",
        "features": ["loops", "arrays_ro", "linear_arith", "termination"],
        "target": ["functional", "termination", "memory_safety"],
        "intent": "edge_case",  # 教学习题
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 1},
        "auxiliary_needed": ["loop_invariant"],
    },
    "frama_c_exo2_solved.c": {
        "name": "frama_c_exo2",
        "features": ["loops", "arrays_ro", "axiomatic", "ghost", "logic_function",
                     "termination"],
        "target": ["functional", "termination", "memory_safety"],
        "intent": "edge_case",  # 教学习题
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 3,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 4},
        "auxiliary_needed": ["loop_invariant", "ghost", "predicate_def"],
    },
    "frama_c_exo3_solved.old.c": {
        "name": "frama_c_exo3_old",
        "features": ["loops", "arrays_ro", "behaviors", "predicate", "termination"],
        "target": ["functional", "termination"],
        "intent": "edge_case",
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 3, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "frama_c_exo3_solved.old.v2.c": {
        "name": "frama_c_exo3_old_v2",
        "features": ["loops", "arrays_ro", "behaviors", "predicate", "termination"],
        "target": ["functional", "termination"],
        "intent": "edge_case",
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 3, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "frama_c_exo3_solved.simplified.c": {
        "name": "frama_c_exo3_simplified",
        "features": ["loops", "arrays_ro", "behaviors", "predicate"],
        "target": ["functional"],
        "intent": "edge_case",
        "difficulty": {"overall": 3, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "string-compare.c": {
        "name": "string_compare",
        "features": ["loops", "strings", "pointers", "behaviors", "termination"],
        "target": ["functional", "termination", "memory_safety"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 3, "control_complexity": 2, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant"],
    },
}


def emit_case_toml(case_dir: Path, case_id: str, meta: dict,
                   upstream_path: str, source_hash: str,
                   strip_lines: list[int], today: str) -> None:
    d = meta["difficulty"]
    target_list = "[" + ", ".join(f'"{t}"' for t in meta["target"]) + "]"
    features_list = "[" + ", ".join(f'"{f}"' for f in meta["features"]) + "]"
    aux_list = "[" + ", ".join(f'"{a}"' for a in meta["auxiliary_needed"]) + "]"
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(meta["name"])}
part          = 3
features      = {features_list}

[provenance]
source             = "frama_c_wp_gallery"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_path)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "algorithm"
intent             = "{meta["intent"]}"

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
rationale   = "wp_gallery 用例：函数 pre/post 视为过程规约（架构 v3+），ground_truth 含完整 ACSL，stripped 为纯 C；agent 任务隐式 = 让 WP+RTE 全过。"

[variants.strip_policy.classified_by]
type              = "human"
reviewed_by_human = true
classified_at     = "{today}"
"""
    (case_dir / "case.toml").write_text(content)


def ingest(source_dir: Path, output_dir: Path) -> int:
    if not source_dir.exists():
        print(f"error: source dir not found: {source_dir}", file=sys.stderr)
        return 0

    today = date.today().isoformat()
    output_dir.mkdir(parents=True, exist_ok=True)
    count = 0

    for src_filename, meta in WP_GALLERY_METADATA.items():
        src_path = source_dir / src_filename
        if not src_path.exists():
            print(f"warn: missing source: {src_path}", file=sys.stderr)
            continue

        case_name = meta["name"]
        case_id = f"part3_curated/frama_c_wp_gallery/{case_name}"
        case_dir = output_dir / case_name
        case_dir.mkdir(parents=True, exist_ok=True)

        # ground_truth.c
        gt_bytes = src_path.read_bytes()
        gt_path = case_dir / "ground_truth.c"
        gt_path.write_bytes(gt_bytes)

        # Analyze ACSL blocks
        blocks = find_acsl_blocks(gt_bytes)
        source_hash = compute_hash(gt_bytes)
        strip_lines = [b.start_line for b in blocks]

        # stripped.c — delete ALL ACSL blocks
        stripped_path = case_dir / "stripped.c"
        if strip_lines:
            strip(gt_path, stripped_path, strip_lines, source_hash)
        else:
            stripped_path.write_bytes(gt_bytes)

        # case.toml
        emit_case_toml(case_dir, case_id, meta, src_filename,
                       source_hash, strip_lines, today)

        print(f"  case {case_id}: {len(blocks)} ACSL blocks, {len(gt_bytes)} bytes",
              file=sys.stderr)
        count += 1

    return count


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_FRAMA_C_SOURCE)
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_dir)
    print(f"\ningested {n} wp_gallery cases to {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
