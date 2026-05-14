#!/usr/bin/env python3
"""Migrate fv-core/tests/benchmark/ contents into FermatVerification-bench.

Per architecture §10.2 migration rules:
- verification/L1-L6 → part3_curated/inhouse_l<N>_<name>/
- cil_features/ → 按内容拆 part 1/2/3
- regression/ → part1_c_features/inhouse_regression/
- x509/ → 跳过（ANSSI part 4 已覆盖）

Each L<N> case maps to specific features + difficulty per the L-level semantics
(L1 pure / L2 loops / L3 arrays_ro / L4 arrays_mut / L5 pointers / L6 callers).

cil_features per-file classification table is maintained inline (CIL_FEATURES_META).

Usage:
    ingest_fv_core_inhouse.py [--source-dir DIR]
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

DEFAULT_SOURCE = Path(
    "/home/haokun/projects/FermatVerification-fcil/fv-core/tests/benchmark"
)

UPSTREAM_URL = "https://github.com/lihaokun/FermatVerification"
UPSTREAM_TOOL_HINT = "fv-core (本仓库 inhouse)"
LICENSE = "MIT"  # fv-core 本仓库默认许可
LICENSE_FILE = "LICENSE"  # 引用本仓库 LICENSE


# L1-L6 metadata defaults
L_LEVEL_META = {
    "L1_pure": {
        "source": "inhouse_l1_pure",
        "features": ["pure_arith"],
        "target": ["functional"],
        "difficulty": {"overall": 1, "proof_size": 0, "math_theory": 1,
                       "data_complexity": 0, "control_complexity": 0, "spec_complexity": 1},
        "auxiliary": [],
    },
    "L2_loops": {
        "source": "inhouse_l2_loops",
        "features": ["loops", "linear_arith"],
        "target": ["functional", "termination"],
        "difficulty": {"overall": 2, "proof_size": 1, "math_theory": 1,
                       "data_complexity": 1, "control_complexity": 2, "spec_complexity": 1},
        "auxiliary": ["loop_invariant"],
    },
    "L3_readonly_arrays": {
        "source": "inhouse_l3_readonly_arrays",
        "features": ["loops", "arrays_ro", "linear_arith"],
        "target": ["functional"],
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary": ["loop_invariant"],
    },
    "L4_mutable_arrays": {
        "source": "inhouse_l4_mutable_arrays",
        "features": ["loops", "arrays_mut", "linear_arith"],
        "target": ["functional", "memory_safety"],
        "difficulty": {"overall": 3, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary": ["loop_invariant"],
    },
    "L5_pointers": {
        "source": "inhouse_l5_pointers",
        "features": ["pointers"],
        "target": ["functional", "memory_safety"],
        "difficulty": {"overall": 4, "proof_size": 1, "math_theory": 1,
                       "data_complexity": 3, "control_complexity": 1, "spec_complexity": 2},
        "auxiliary": ["loop_invariant"],
    },
    "L6_callers": {
        "source": "inhouse_l6_callers",
        "features": ["callers"],
        "target": ["functional"],
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 1,
                       "data_complexity": 1, "control_complexity": 1, "spec_complexity": 2},
        "auxiliary": [],
    },
}


# cil_features 每文件分类 → (part, sub_category, features, intent, difficulty.overall)
# Part 1 = C feature stress; Part 2 = ACSL feature demo; Part 3 = real algorithm
CIL_FEATURES_META: dict[str, dict] = {
    # Part 3 — real algorithms
    "acsl_by_example.c": {"part": 3, "features": ["loops", "arrays_ro", "predicate"], "intent": "functional_correctness", "difficulty": 3},
    "array_utils.c":     {"part": 3, "features": ["loops", "arrays_mut", "pointers"], "intent": "functional_correctness", "difficulty": 3},
    "bsearch.c":         {"part": 3, "features": ["loops", "arrays_ro", "linear_arith"], "intent": "functional_correctness", "difficulty": 3},
    "mini_sort.c":       {"part": 3, "features": ["loops", "nested_loops", "arrays_mut"], "intent": "functional_correctness", "difficulty": 4},
    "string_utils.c":    {"part": 3, "features": ["loops", "strings", "pointers"], "intent": "functional_correctness", "difficulty": 3},
    "test_abs.c":        {"part": 3, "features": ["pure_arith"], "intent": "functional_correctness", "difficulty": 1},
    "test_comprehensive.c": {"part": 3, "features": ["loops", "arrays_mut"], "intent": "functional_correctness", "difficulty": 3},
    "test_phase2.c":     {"part": 3, "features": ["loops", "arrays_ro"], "intent": "functional_correctness", "difficulty": 2},

    # Part 2 — ACSL feature demos
    "annot.c":              {"part": 2, "features": ["behaviors"], "intent": "feature_demo", "difficulty": 1},
    "assigns.c":            {"part": 2, "features": ["behaviors"], "intent": "feature_demo", "difficulty": 1},
    "assume.c":             {"part": 2, "features": ["behaviors"], "intent": "feature_demo", "difficulty": 1},
    "behavior_assert.c":    {"part": 2, "features": ["behaviors"], "intent": "feature_demo", "difficulty": 2},
    "complete_behaviors.c": {"part": 2, "features": ["behaviors"], "intent": "feature_demo", "difficulty": 2},
    "ghost.c":              {"part": 2, "features": ["ghost"], "intent": "feature_demo", "difficulty": 2},
    "logic_def.c":          {"part": 2, "features": ["logic_function", "predicate"], "intent": "feature_demo", "difficulty": 2},
    "terminates.c":         {"part": 2, "features": ["termination"], "intent": "feature_demo", "difficulty": 1},
    "enum.c":               {"part": 2, "features": ["logic_function"], "intent": "feature_demo", "difficulty": 1},

    # Part 1 — C feature stress
    "array_formals.c":            {"part": 1, "features": ["arrays_ro"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "array_size.c":               {"part": 1, "features": ["arrays_ro"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "array_typedef.c":            {"part": 1, "features": ["arrays_ro", "nested_struct"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "binary_logic_op.c":          {"part": 1, "features": ["pure_arith"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "binary_op.c":                {"part": 1, "features": ["pure_arith"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "bool.c":                     {"part": 1, "features": [], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "boolean_ops.c":              {"part": 1, "features": [], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "branch.c":                   {"part": 1, "features": [], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "char_ampamp.c":              {"part": 1, "features": ["strings"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "dowhilezero.c":              {"part": 1, "features": ["loops"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "empty_cond.c":               {"part": 1, "features": ["loops"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "export_test.c":              {"part": 1, "features": [], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "find_enclosing_loop.c":      {"part": 1, "features": ["loops"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "parenthesis.c":              {"part": 1, "features": ["pure_arith"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "relations.c":                {"part": 1, "features": ["pure_arith"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "struct_with_invalid_field.c": {"part": 1, "features": ["nested_struct"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "test_iterative_raw.c":       {"part": 1, "features": ["loops"], "intent": "parser_stress", "constructs": [], "difficulty": 1},
    "test_sandbox.c":             {"part": 1, "features": [], "intent": "parser_stress", "constructs": [], "difficulty": 1},

    # SKIP — covered by ANSSI x509 part 4
    "x509-submodule-2_no_assert.c": {"skip": True},
}


def _safe_name(stem: str) -> str:
    return stem.replace("-", "_").replace(".", "_").replace(" ", "_")


def emit_part3_case(case_dir: Path, case_id: str, name: str,
                    source: str, upstream_path: str, meta: dict,
                    source_hash: str, strip_lines: list[int], today: str) -> None:
    d = meta["difficulty"]
    features_list = "[" + ", ".join(f'"{f}"' for f in meta["features"]) + "]"
    target_list = "[" + ", ".join(f'"{t}"' for t in meta["target"]) + "]"
    aux_list = "[" + ", ".join(f'"{a}"' for a in meta["auxiliary"]) + "]"
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 3
features      = {features_list}

[provenance]
source             = "{source}"
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
rationale   = "fv-core inhouse case，从原 FermatVerification 主仓库迁移。架构 v3+ 函数 pre/post 视为过程规约。"

[variants.strip_policy.classified_by]
type              = "human"
reviewed_by_human = true
classified_at     = "{today}"
"""
    (case_dir / "case.toml").write_text(content)


def emit_part2_case(case_dir: Path, case_id: str, name: str,
                    upstream_path: str, features: list[str], intent: str,
                    difficulty: int, today: str, entry_file: str) -> None:
    features_list = "[" + ", ".join(f'"{f}"' for f in features) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 2
features      = {features_list}

[provenance]
source             = "inhouse"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_path)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "acsl_feature_demo"
intent             = "{intent}"

[files]
entry_files = ["{entry_file}"]

[difficulty]
overall            = {difficulty}
proof_size         = 1
math_theory        = 1
data_complexity    = 1
control_complexity = 1
spec_complexity    = 2
auxiliary_needed   = []

[difficulty.scored_by]
type      = "human"
scored_at = "{today}"

[parse]
constructs_used = []
known_issues    = []
notes           = "fv-core inhouse ACSL feature case，从原 FermatVerification 主仓库迁移。"
"""
    (case_dir / "case.toml").write_text(content)


def emit_part1_case(case_dir: Path, case_id: str, name: str,
                    upstream_path: str, features: list[str], constructs: list[str],
                    quality: str, intent: str, difficulty: int,
                    today: str, entry_file: str) -> None:
    features_list = "[" + ", ".join(f'"{f}"' for f in features) + "]"
    constructs_list = "[" + ", ".join(f'"{c}"' for c in constructs) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 1
features      = {features_list}

[provenance]
source             = "inhouse"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_path)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "{quality}"
intent             = "{intent}"

[files]
entry_files = ["{entry_file}"]

[difficulty]
overall            = {difficulty}
proof_size         = 0
math_theory        = 0
data_complexity    = 1
control_complexity = 1
spec_complexity    = 0
auxiliary_needed   = []

[difficulty.scored_by]
type      = "human"
scored_at = "{today}"

[parse]
constructs_used = {constructs_list}
known_issues    = []
notes           = "fv-core inhouse C feature case，从原 FermatVerification 主仓库迁移。"
"""
    (case_dir / "case.toml").write_text(content)


def ingest_verification(source_dir: Path, today: str) -> int:
    """Ingest L1-L6 verification cases to Part 3."""
    total = 0
    for level_name, meta in L_LEVEL_META.items():
        ld = source_dir / "verification" / level_name
        if not ld.exists():
            continue
        out_dir = REPO_ROOT / "cases" / "part3_curated" / meta["source"]
        out_dir.mkdir(parents=True, exist_ok=True)

        for src in sorted(ld.glob("*.c")):
            cname = _safe_name(src.stem)
            case_id = f"part3_curated/{meta['source']}/{cname}"
            case_dir = out_dir / cname
            case_dir.mkdir(parents=True, exist_ok=True)

            gt_bytes = src.read_bytes()
            (case_dir / "ground_truth.c").write_bytes(gt_bytes)

            blocks = find_acsl_blocks(gt_bytes)
            source_hash = compute_hash(gt_bytes)
            strip_lines = [b.start_line for b in blocks]

            stripped_path = case_dir / "stripped.c"
            if strip_lines:
                strip(case_dir / "ground_truth.c", stripped_path, strip_lines, source_hash)
            else:
                stripped_path.write_bytes(gt_bytes)

            upstream_path = f"fv-core/tests/benchmark/verification/{level_name}/{src.name}"
            emit_part3_case(
                case_dir=case_dir, case_id=case_id, name=cname,
                source=meta["source"], upstream_path=upstream_path,
                meta=meta, source_hash=source_hash,
                strip_lines=strip_lines, today=today,
            )
            total += 1
    return total


def ingest_cil_features(source_dir: Path, today: str) -> int:
    """Ingest cil_features (mixed Part 1/2/3 per classification table)."""
    cf_dir = source_dir / "cil_features"
    if not cf_dir.exists():
        return 0

    total = 0
    for src in sorted(cf_dir.glob("*.c")):
        meta = CIL_FEATURES_META.get(src.name)
        if meta is None:
            print(f"warn: no metadata for cil_features/{src.name}, skip", file=sys.stderr)
            continue
        if meta.get("skip"):
            print(f"  skip cil_features/{src.name}: {meta.get('reason', 'covered elsewhere')}", file=sys.stderr)
            continue

        cname = _safe_name(src.stem)
        part = meta["part"]

        if part == 3:
            out_dir = REPO_ROOT / "cases" / "part3_curated" / f"inhouse_{cname}"
            out_dir.mkdir(parents=True, exist_ok=True)
            case_id = f"part3_curated/inhouse_{cname}"
            case_dir = out_dir
            case_dir.mkdir(parents=True, exist_ok=True)

            gt_bytes = src.read_bytes()
            (case_dir / "ground_truth.c").write_bytes(gt_bytes)
            blocks = find_acsl_blocks(gt_bytes)
            source_hash = compute_hash(gt_bytes)
            strip_lines = [b.start_line for b in blocks]
            stripped_path = case_dir / "stripped.c"
            if strip_lines:
                strip(case_dir / "ground_truth.c", stripped_path, strip_lines, source_hash)
            else:
                stripped_path.write_bytes(gt_bytes)

            # part3 case meta inline (not in L_LEVEL_META)
            d3 = meta["difficulty"]
            meta3 = {
                "features": meta["features"],
                "target": ["functional"],
                "difficulty": {"overall": d3, "proof_size": min(d3, 3),
                               "math_theory": 1, "data_complexity": 2,
                               "control_complexity": 2, "spec_complexity": 2},
                "auxiliary": ["loop_invariant"] if "loops" in meta["features"] else [],
            }
            emit_part3_case(
                case_dir=case_dir, case_id=case_id, name=cname,
                source="inhouse", upstream_path=f"fv-core/tests/benchmark/cil_features/{src.name}",
                meta=meta3, source_hash=source_hash,
                strip_lines=strip_lines, today=today,
            )
        elif part == 2:
            out_dir = REPO_ROOT / "cases" / "part2_acsl_features" / "inhouse"
            out_dir.mkdir(parents=True, exist_ok=True)
            case_id = f"part2_acsl_features/inhouse/{cname}"
            case_dir = out_dir / cname
            case_dir.mkdir(parents=True, exist_ok=True)
            entry_file = src.name
            (case_dir / entry_file).write_bytes(src.read_bytes())

            emit_part2_case(
                case_dir=case_dir, case_id=case_id, name=cname,
                upstream_path=f"fv-core/tests/benchmark/cil_features/{src.name}",
                features=meta["features"], intent=meta["intent"],
                difficulty=meta["difficulty"], today=today,
                entry_file=entry_file,
            )
        elif part == 1:
            out_dir = REPO_ROOT / "cases" / "part1_c_features" / "inhouse"
            out_dir.mkdir(parents=True, exist_ok=True)
            case_id = f"part1_c_features/inhouse/{cname}"
            case_dir = out_dir / cname
            case_dir.mkdir(parents=True, exist_ok=True)
            entry_file = src.name
            (case_dir / entry_file).write_bytes(src.read_bytes())

            emit_part1_case(
                case_dir=case_dir, case_id=case_id, name=cname,
                upstream_path=f"fv-core/tests/benchmark/cil_features/{src.name}",
                features=meta["features"], constructs=meta["constructs"],
                quality="frontend_stress", intent=meta["intent"],
                difficulty=meta["difficulty"], today=today,
                entry_file=entry_file,
            )

        total += 1
    return total


def ingest_regression(source_dir: Path, today: str) -> int:
    """Ingest fv-core regression to Part 1 with quality=synthetic_corner."""
    reg_dir = source_dir / "regression"
    if not reg_dir.exists():
        return 0
    total = 0
    out_dir = REPO_ROOT / "cases" / "part1_c_features" / "inhouse_regression"
    out_dir.mkdir(parents=True, exist_ok=True)

    for src in sorted(reg_dir.glob("*.c")):
        cname = _safe_name(src.stem)
        case_id = f"part1_c_features/inhouse_regression/{cname}"
        case_dir = out_dir / cname
        case_dir.mkdir(parents=True, exist_ok=True)
        entry_file = src.name
        (case_dir / entry_file).write_bytes(src.read_bytes())

        emit_part1_case(
            case_dir=case_dir, case_id=case_id, name=cname,
            upstream_path=f"fv-core/tests/benchmark/regression/{src.name}",
            features=[], constructs=[],
            quality="synthetic_corner", intent="edge_case",
            difficulty=1, today=today,
            entry_file=entry_file,
        )
        total += 1
    return total


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    args = p.parse_args()

    if not args.source_dir.exists():
        print(f"error: source dir not found: {args.source_dir}", file=sys.stderr)
        sys.exit(1)

    today = date.today().isoformat()

    n_v = ingest_verification(args.source_dir, today)
    print(f"\ningested {n_v} L1-L6 verification cases", file=sys.stderr)

    n_c = ingest_cil_features(args.source_dir, today)
    print(f"ingested {n_c} cil_features cases (mixed parts)", file=sys.stderr)

    n_r = ingest_regression(args.source_dir, today)
    print(f"ingested {n_r} regression cases", file=sys.stderr)

    print(f"\ntotal: {n_v + n_c + n_r}", file=sys.stderr)


if __name__ == "__main__":
    main()
