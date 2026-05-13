#!/usr/bin/env python3
"""Ingest Frama-C tests/syntax + tests/cil + tests/spec into Part 1/2.

Frama-C's tests/ directory contains a large collection of C / C+ACSL test
inputs exercising the frontend's coverage. We map three subdirs:

    tests/syntax/    → Part 1 (frontend stress, C syntax)
    tests/cil/       → Part 1 (CIL transformation tests)
    tests/spec/      → Part 2 (ACSL spec tests)

For each .c / .i file in those directories:
  - Generate one case in cases/part{1,2}_{c_features,acsl_features}/<source>/<name>/
  - Skip .ml (OCaml plugins, not C/ACSL inputs)
  - Derive features + constructs_used heuristically from filename

Usage:
    ingest_frama_c_tests.py [--source-dir DIR]
"""

from __future__ import annotations

import argparse
import sys
from datetime import date
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION
from fetch_casp import format_toml_string

DEFAULT_SOURCE = Path(
    "/home/haokun/.opam/frama32/.opam-switch/sources/frama-c.32.0/tests"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases"

UPSTREAM_URL = "https://git.frama-c.com/pub/frama-c"
UPSTREAM_TOOL_HINT = "frama-c 32 (Gallium)"
LICENSE = "LGPL-2.1-only"
LICENSE_FILE = "LICENSES/LGPL-2.1-frama-c.txt"


# Source dir → (part, source, output sub-path, default quality, default intent)
SUBDIR_CONFIG: dict[str, dict] = {
    "syntax": {
        "part": 1,
        "source": "frama_c_syntax",
        "out_subpath": "part1_c_features/frama_c_syntax",
        "quality": "frontend_stress",
        "intent": "parser_stress",
    },
    "cil": {
        "part": 1,
        "source": "frama_c_cil",
        "out_subpath": "part1_c_features/frama_c_cil",
        "quality": "frontend_stress",
        "intent": "parser_stress",
    },
    "spec": {
        "part": 2,
        "source": "frama_c_spec",
        "out_subpath": "part2_acsl_features/frama_c_spec",
        "quality": "acsl_feature_demo",
        "intent": "feature_demo",
    },
}


# Heuristic: filename keyword → ACSL semantic feature (used for spec/ Part 2)
KEYWORD_TO_FEATURE = {
    "ghost": "ghost",
    "lemma": "lemma",
    "axiom": "axiomatic",
    "inductive": "axiomatic",
    "logic": "logic_function",
    "predicate": "predicate",
    "behavior": "behaviors",
    "assumes": "behaviors",
    "decreases": "termination",
    "terminates": "termination",
    "variant": "termination",
    "bitwise": "bitwise",
    "float": "floats",
    "loop": "loops",
    "pointer": "pointers",
    "null": "pointers",
    "array": "arrays_ro",
    "struct": "nested_struct",
    "record": "nested_struct",
}

# Heuristic: filename keyword → C constructs_used (used for syntax/, cil/ Part 1)
KEYWORD_TO_CONSTRUCT = {
    "alignas": "_Alignas",
    "generic": "_Generic",
    "vla": "vla",
    "bitfield": "bitfield",
    "designated": "designated_initializer",
    "stmt_expr": "statement_expression",
    "attribute": "gcc_extension",
    "attr_": "gcc_extension",
    "asm": "gcc_extension",
    "_static_assert": "c11_only",
    "thread_local": "c11_only",
}

# Heuristic: filename keyword → C semantic feature (Part 1 features field)
KEYWORD_TO_C_FEATURE = {
    "array": "arrays_ro",
    "string": "strings",
    "ptr": "pointers",
    "pointer": "pointers",
    "addr": "pointers",
    "struct": "nested_struct",
    "union": "nested_struct",
    "bitop": "bitwise",
    "bit_": "bitwise",
    "shift": "bitwise",
    "float": "floats",
    "double": "floats",
    "loop": "loops",
    "while": "loops",
    "recur": "recursion",
}


def derive_tags(filename_lower: str, mapping: dict[str, str]) -> list[str]:
    tags = set()
    for kw, tag in mapping.items():
        if kw in filename_lower:
            tags.add(tag)
    return sorted(tags)


def _safe_name(stem: str) -> str:
    return stem.replace("-", "_").replace(".", "_").replace(" ", "_")


def emit_part1_case_toml(case_dir: Path, case_id: str, name: str,
                         entry_file: str, upstream_filename: str,
                         source: str, quality: str, intent: str,
                         features: list[str], constructs: list[str],
                         today: str) -> None:
    features_list = "[" + ", ".join(f'"{f}"' for f in features) + "]"
    constructs_list = "[" + ", ".join(f'"{c}"' for c in constructs) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 1
features      = {features_list}

[provenance]
source             = "{source}"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_filename)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "{quality}"
intent             = "{intent}"

[files]
entry_files = ["{entry_file}"]

[difficulty]
overall            = 1
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
notes           = "Frama-C tests/{source.replace('frama_c_', '')} — frontend 用例。features / constructs_used 由文件名启发式推导。"
"""
    (case_dir / "case.toml").write_text(content)


def emit_part2_case_toml(case_dir: Path, case_id: str, name: str,
                         entry_file: str, upstream_filename: str,
                         source: str, quality: str, intent: str,
                         features: list[str], today: str) -> None:
    features_list = "[" + ", ".join(f'"{f}"' for f in features) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 2
features      = {features_list}

[provenance]
source             = "{source}"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_filename)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "{quality}"
intent             = "{intent}"

[files]
entry_files = ["{entry_file}"]

[difficulty]
overall            = 2
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
notes           = "Frama-C tests/spec — ACSL 特性测试。features 由文件名启发式推导。"
"""
    (case_dir / "case.toml").write_text(content)


def ingest(source_dir: Path, output_root: Path) -> int:
    if not source_dir.exists():
        print(f"error: source dir not found: {source_dir}", file=sys.stderr)
        return 0

    today = date.today().isoformat()
    total = 0

    for sub_name, cfg in SUBDIR_CONFIG.items():
        sub_path = source_dir / sub_name
        if not sub_path.exists():
            print(f"warn: missing subdir: {sub_path}", file=sys.stderr)
            continue

        files = sorted(list(sub_path.glob("*.c")) + list(sub_path.glob("*.i")))
        print(f"tests/{sub_name}: {len(files)} .c/.i files", file=sys.stderr)

        out_dir = output_root / cfg["out_subpath"]
        out_dir.mkdir(parents=True, exist_ok=True)

        for src in files:
            if not src.is_file():
                continue
            cname = _safe_name(src.stem)
            case_id = f"{cfg['out_subpath']}/{cname}"
            case_dir = out_dir / cname
            case_dir.mkdir(parents=True, exist_ok=True)

            entry_file = src.name
            (case_dir / entry_file).write_bytes(src.read_bytes())

            fname_low = src.name.lower()
            upstream_filename = f"tests/{sub_name}/{src.name}"

            if cfg["part"] == 1:
                features = derive_tags(fname_low, KEYWORD_TO_C_FEATURE)
                constructs = derive_tags(fname_low, KEYWORD_TO_CONSTRUCT)
                emit_part1_case_toml(
                    case_dir=case_dir, case_id=case_id, name=cname,
                    entry_file=entry_file, upstream_filename=upstream_filename,
                    source=cfg["source"], quality=cfg["quality"],
                    intent=cfg["intent"], features=features,
                    constructs=constructs, today=today,
                )
            else:  # part == 2
                features = derive_tags(fname_low, KEYWORD_TO_FEATURE)
                emit_part2_case_toml(
                    case_dir=case_dir, case_id=case_id, name=cname,
                    entry_file=entry_file, upstream_filename=upstream_filename,
                    source=cfg["source"], quality=cfg["quality"],
                    intent=cfg["intent"], features=features, today=today,
                )

            total += 1

    return total


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    p.add_argument("--output-root", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_root)
    print(f"\ningested {n} frama-c test cases", file=sys.stderr)


if __name__ == "__main__":
    main()
