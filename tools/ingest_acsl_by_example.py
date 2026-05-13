#!/usr/bin/env python3
"""Ingest Fraunhofer ACSL by Example into cases/part3_curated/acsl_by_example/.

ACSL by Example's structure:
    StandardAlgorithms/
        typedefs.h          — shared types (value_type / size_type / bool)
        Logic/              — shared ACSL lemma / predicate files (60+ .acsl)
        BinarySearch/       — algorithm group
            binary_search.c — implementation
            binary_search.h — ACSL contract (requires/ensures/...)
            lower_bound.h   — dep ACSL contract
            ...
        MinMax/ / Mutating/ / Nonmutating/ / Heap/ / Numeric/ / Sorting/ / Stack/

Each algorithm's `.c` file is the unit of verification. The ACSL contracts live
in the corresponding `.h` files; shared predicates / lemmas live in `Logic/*.acsl`.

This script:
  - For each `.c` file in 8 groups, recursively resolves `#include "..."` to
    produce a flat ground_truth.c with all dependencies inlined
  - System `<...>` includes are kept as-is (consumer's preprocessor handles)
  - Same file seen twice → second time skipped (acts as include guard)
  - Runs strip_acsl to produce stripped.c (deletes all ACSL blocks)
  - Generates case.toml with group-level defaults + name

Usage:
    ingest_acsl_by_example.py [--source-dir DIR]
"""

from __future__ import annotations

import argparse
import re
import sys
from datetime import date
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION
from strip_acsl import find_acsl_blocks, compute_hash, strip
from fetch_casp import format_toml_string

DEFAULT_SOURCE = Path(
    "/home/haokun/projects/FermatVerification-fcil/bench-research/"
    "acsl-by-example/StandardAlgorithms"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part3_curated" / "acsl_by_example"

UPSTREAM_URL = "https://github.com/fraunhoferfokus/acsl-by-example"
UPSTREAM_TOOL_HINT = "frama-c 32 (Gallium)"
LICENSE = "MIT"
LICENSE_FILE = "LICENSES/MIT-fraunhofer-fokus.txt"

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"]+)"\s*$')


# Group-level metadata (8 groups). Per-case can override if needed (none for v0.1).
GROUP_METADATA = {
    "BinarySearch": {
        "features": ["loops", "arrays_ro", "linear_arith", "predicate"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "MinMax": {
        "features": ["loops", "arrays_ro", "linear_arith"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 2, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary_needed": ["loop_invariant"],
    },
    "Mutating": {
        "features": ["loops", "arrays_mut", "pointers", "predicate"],
        "target": ["functional", "memory_safety"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 3, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "Nonmutating": {
        "features": ["loops", "arrays_ro", "predicate"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 2, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 2},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "Heap": {
        "features": ["loops", "arrays_mut", "predicate", "axiomatic"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 3, "control_complexity": 3, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def", "lemma"],
    },
    "Numeric": {
        "features": ["loops", "arrays_ro", "linear_arith", "logic_function"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 1, "math_theory": 2,
                       "data_complexity": 2, "control_complexity": 2, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "Sorting": {
        "features": ["loops", "nested_loops", "arrays_mut", "predicate"],
        "target": ["functional"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 4, "proof_size": 2, "math_theory": 2,
                       "data_complexity": 3, "control_complexity": 3, "spec_complexity": 3},
        "auxiliary_needed": ["loop_invariant", "predicate_def"],
    },
    "Stack": {
        "features": ["pointers", "nested_struct", "predicate", "behaviors"],
        "target": ["functional", "memory_safety"],
        "intent": "functional_correctness",
        "difficulty": {"overall": 3, "proof_size": 2, "math_theory": 1,
                       "data_complexity": 4, "control_complexity": 2, "spec_complexity": 3},
        "auxiliary_needed": ["predicate_def", "lemma"],
    },
}


def find_in_search_dirs(name: str, dirs: list[Path]) -> Path | None:
    for d in dirs:
        p = d / name
        if p.exists():
            return p
    return None


def resolve_includes(src: Path, search_dirs: list[Path],
                     seen: set[Path] | None = None) -> str:
    """Recursively expand #include "..." for src.

    System <...> includes are kept as-is. Each file seen twice is skipped
    (acts as include guard).
    """
    if seen is None:
        seen = set()
    rp = src.resolve()
    if rp in seen:
        return ""
    seen.add(rp)

    out = [f'/* ===== include: {src.name} ===== */']
    for line in src.read_text(errors="replace").split("\n"):
        m = INCLUDE_RE.match(line)
        if m:
            inc_name = m.group(1)
            inc_path = find_in_search_dirs(inc_name, search_dirs)
            if inc_path is not None:
                inner = resolve_includes(inc_path, search_dirs, seen)
                if inner:
                    out.append(inner)
                continue
            else:
                out.append(f"/* could not resolve: {line} */")
                continue
        out.append(line)
    return "\n".join(out)


def case_name(c_file: Path) -> str:
    """Derive case dir name from .c file basename."""
    return c_file.stem


def iter_cases(source_dir: Path) -> list[tuple[str, Path]]:
    """Return [(group, c_file), ...] for all algorithm .c files we'll ingest."""
    result = []
    for group in GROUP_METADATA:
        gdir = source_dir / group
        if not gdir.exists():
            continue
        for c in sorted(gdir.glob("*.c")):
            # Skip _test.cpp / _test.c files (those are test drivers, not algos)
            if c.name.endswith("_test.c") or c.name.endswith("_test.cpp"):
                continue
            result.append((group, c))
    return result


def emit_case_toml(case_dir: Path, case_id: str, name: str, group: str,
                   upstream_path: str, source_hash: str, strip_lines: list[int],
                   today: str) -> None:
    meta = GROUP_METADATA[group]
    d = meta["difficulty"]
    target_list = "[" + ", ".join(f'"{t}"' for t in meta["target"]) + "]"
    features_list = "[" + ", ".join(f'"{f}"' for f in meta["features"]) + "]"
    aux_list = "[" + ", ".join(f'"{a}"' for a in meta["auxiliary_needed"]) + "]"
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 3
features      = {features_list}

[provenance]
source             = "acsl_by_example"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_path)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "textbook"
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
rationale   = "ACSL by Example {group} 用例：#include 展平 → 单文件 ground_truth；函数 pre/post 视为过程规约（架构 v3+），stripped 为纯 C；agent 任务隐式 = 让 WP+RTE 全过。"

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

    # Include search order: case's own group dir first, then top dir, then Logic/
    logic_dir = source_dir / "Logic"
    count = 0

    cases = iter_cases(source_dir)
    print(f"discovered {len(cases)} candidate .c files across {len(GROUP_METADATA)} groups",
          file=sys.stderr)

    for group, c_file in cases:
        group_dir = c_file.parent
        search_dirs = [group_dir, source_dir, logic_dir]
        cname = case_name(c_file)
        case_id = f"part3_curated/acsl_by_example/{group}/{cname}"
        case_dir = output_dir / group / cname
        case_dir.mkdir(parents=True, exist_ok=True)

        # Resolve includes → flatten
        flat_text = resolve_includes(c_file, search_dirs)
        gt_bytes = flat_text.encode("utf-8")
        gt_path = case_dir / "ground_truth.c"
        gt_path.write_bytes(gt_bytes)

        # Compute hash + find ACSL blocks
        source_hash = compute_hash(gt_bytes)
        blocks = find_acsl_blocks(gt_bytes)
        strip_lines = [b.start_line for b in blocks]

        # stripped.c
        stripped_path = case_dir / "stripped.c"
        if strip_lines:
            strip(gt_path, stripped_path, strip_lines, source_hash)
        else:
            stripped_path.write_bytes(gt_bytes)

        # case.toml
        upstream_path = f"StandardAlgorithms/{group}/{c_file.name}"
        emit_case_toml(case_dir, case_id, cname, group, upstream_path,
                       source_hash, strip_lines, today)

        print(f"  case {case_id}: flat={len(gt_bytes)}B, {len(blocks)} ACSL blocks",
              file=sys.stderr)
        count += 1

    return count


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_dir)
    print(f"\ningested {n} acsl-by-example cases to {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
