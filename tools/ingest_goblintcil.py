#!/usr/bin/env python3
"""Ingest GoblintCil test corpus into cases/part1_c_features/goblintcil/.

GoblintCil is a fork of CIL maintained by the Goblint analyzer team. Its
test/small1/ + test/small2/ contain 662 small C files that historically
exposed CIL parser corner cases (GCC extensions, alignment, designated
initializers, _Alignas, _Generic, bitfields, VLA, etc.).

These cases have **no ACSL** — they are pure C frontend stress tests.

Part 1 schema: [parse] segment + features. No [variants] / [verification].

This script:
  - Copies each .c file to cases/part1_c_features/goblintcil/<smallN>/<name>/
  - Derives `constructs_used` heuristically from filename keywords
  - Derives `features` (semantic) similarly when filename hints at C constructs
  - Marks quality=frontend_stress, intent=parser_stress

Usage:
    ingest_goblintcil.py [--source-dir DIR]
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
    "/home/haokun/projects/FermatVerification-fcil/bench-research/goblintcil/test"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part1_c_features" / "goblintcil"

UPSTREAM_URL = "https://github.com/goblint/cil"
UPSTREAM_TOOL_HINT = "goblintcil parser (BSD-3 fork of CIL)"
LICENSE = "BSD-3-Clause"
LICENSE_FILE = "LICENSES/BSD-3-goblint.txt"


# constructs_used 词表关键词 → 标签 (§15.7)
KEYWORD_TO_CONSTRUCT = {
    "alignas": "_Alignas",
    "generic": "_Generic",
    "vla": "vla",
    "bitfield": "bitfield",
    "designated": "designated_initializer",
    "stmt_expr": "statement_expression",
    "statement_expr": "statement_expression",
    "attribute": "gcc_extension",
    "attr_": "gcc_extension",
    "asm": "gcc_extension",
    "asmstr": "gcc_extension",
    "longlong": "c99_only",
    "complex": "c99_only",
    "_Static_assert": "c11_only",
    "static_assert": "c11_only",
    "thread": "c11_only",
}

# features 词表（语义层）— 从文件名识别
KEYWORD_TO_FEATURE = {
    "array": "arrays_ro",
    "string": "strings",
    "ptr": "pointers",
    "pointer": "pointers",
    "addr": "pointers",
    "deref": "pointers",
    "struct": "nested_struct",
    "union": "nested_struct",
    "bitop": "bitwise",
    "bit_": "bitwise",
    "shift": "bitwise",
    "float": "floats",
    "double": "floats",
    "loop": "loops",
    "while": "loops",
    "for_": "loops",
    "recur": "recursion",
}


def derive_constructs(filename_lower: str) -> list[str]:
    cs = set()
    for kw, c in KEYWORD_TO_CONSTRUCT.items():
        if kw in filename_lower:
            cs.add(c)
    return sorted(cs)


def derive_features(filename_lower: str) -> list[str]:
    fs = set()
    for kw, f in KEYWORD_TO_FEATURE.items():
        if kw in filename_lower:
            fs.add(f)
    return sorted(fs)


def emit_case_toml(case_dir: Path, case_id: str, name: str, entry_file: str,
                   upstream_filename: str, features: list[str],
                   constructs: list[str], today: str) -> None:
    features_list = "[" + ", ".join(f'"{f}"' for f in features) + "]"
    constructs_list = "[" + ", ".join(f'"{c}"' for c in constructs) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 1
features      = {features_list}

[provenance]
source             = "goblintcil"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_filename)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "frontend_stress"
intent             = "parser_stress"

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
notes           = "GoblintCil parser stress; 无 ACSL；constructs_used 由文件名启发式推导。"
"""
    (case_dir / "case.toml").write_text(content)


def _safe_name(stem: str) -> str:
    """Sanitize a filename stem for use as directory/identifier."""
    safe = stem.replace("-", "_").replace(".", "_").replace(" ", "_")
    # Avoid leading digit (TOML accepts, but cleaner for naming)
    return safe


def ingest(source_dir: Path, output_dir: Path) -> int:
    if not source_dir.exists():
        print(f"error: source dir not found: {source_dir}", file=sys.stderr)
        return 0

    today = date.today().isoformat()
    output_dir.mkdir(parents=True, exist_ok=True)

    total = 0
    for sub in ("small1", "small2"):
        subdir = source_dir / sub
        if not subdir.exists():
            print(f"warn: missing source subdir: {subdir}", file=sys.stderr)
            continue

        files = sorted(subdir.glob("*.c"))
        print(f"{sub}: {len(files)} .c files", file=sys.stderr)

        for src in files:
            if not src.is_file():
                continue
            cname = _safe_name(src.stem)
            case_id = f"part1_c_features/goblintcil/{sub}/{cname}"
            case_dir = output_dir / sub / cname
            case_dir.mkdir(parents=True, exist_ok=True)

            entry_file = src.name  # keep original ".c" extension
            (case_dir / entry_file).write_bytes(src.read_bytes())

            fname_low = src.name.lower()
            features = derive_features(fname_low)
            constructs = derive_constructs(fname_low)

            upstream_filename = f"test/{sub}/{src.name}"
            emit_case_toml(
                case_dir=case_dir,
                case_id=case_id,
                name=cname,
                entry_file=entry_file,
                upstream_filename=upstream_filename,
                features=features,
                constructs=constructs,
                today=today,
            )
            total += 1

    return total


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_dir)
    print(f"\ningested {n} goblintcil cases to {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
