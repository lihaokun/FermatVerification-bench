#!/usr/bin/env python3
"""Ingest Frama-C wp_acsl ACSL feature tests into cases/part2_acsl_features/frama_c_wp_acsl/.

Each `.c` / `.i` file in frama-c's wp_acsl test directory tests a specific ACSL
feature (ghost, axiomatic, behaviors, decreases, lemma, predicate, etc.).

Part 2 cases have schema: [parse] section, no [variants] / [verification].

Features are derived heuristically from filename keywords. Difficulty defaults
to overall=2 (these are small unit tests); per-file overrides could refine.

Usage:
    ingest_wp_acsl.py [--source-dir DIR]
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
    "/home/haokun/.opam/frama32/.opam-switch/sources/frama-c.32.0/"
    "src/plugins/wp/tests/wp_acsl"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part2_acsl_features" / "frama_c_wp_acsl"

UPSTREAM_URL = "https://git.frama-c.com/pub/frama-c"
UPSTREAM_TOOL_HINT = "frama-c 32 (Gallium)"
LICENSE = "LGPL-2.1-only"
LICENSE_FILE = "LICENSES/LGPL-2.1-frama-c.txt"


# Heuristic feature derivation by filename keywords.
# Maps keyword → feature tag. Multiple keywords may match (case names contain
# multiple substrings).
KEYWORD_TO_FEATURE = {
    "ghost": "ghost",
    "lemma": "lemma",
    "axiom": "axiomatic",
    "inductive": "axiomatic",  # inductive types are an axiomatic feature
    "logic": "logic_function",
    "predicate": "predicate",
    "behavior": "behaviors",
    "assumes": "behaviors",
    "decreases": "termination",
    "terminates": "termination",
    "variant": "termination",
    "bitwise": "bitwise",
    "bit_test": "bitwise",
    "float": "floats",
    "real": "floats",
    "sqrt": "floats",
    "loop": "loops",
    "pointer": "pointers",
    "null": "pointers",
    "object_pointer": "pointers",
    "shift_array": "arrays_mut",
    "assign_array": "arrays_mut",
    "array": "arrays_ro",
    "struct": "nested_struct",
    "record": "nested_struct",
    "intbool": "pure_arith",
    "arith": "pure_arith",
    "div_mod": "pure_arith",
    "sizeof": "pure_arith",
}


def derive_features(filename_lower: str) -> list[str]:
    """Return sorted, deduplicated list of features matching keywords in name."""
    feats = set()
    for kw, feat in KEYWORD_TO_FEATURE.items():
        if kw in filename_lower:
            feats.add(feat)
    return sorted(feats)


def emit_case_toml(case_dir: Path, case_id: str, name: str, entry_file: str,
                   upstream_filename: str, features: list[str], today: str) -> None:
    features_list = "[" + ", ".join(f'"{f}"' for f in features) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 2
features      = {features_list}

[provenance]
source             = "frama_c_wp_acsl"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_filename)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "acsl_feature_demo"
intent             = "feature_demo"

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
notes           = "Frama-C wp_acsl ACSL 特性单元测试。features 由文件名启发式推导。"
"""
    (case_dir / "case.toml").write_text(content)


def ingest(source_dir: Path, output_dir: Path) -> int:
    if not source_dir.exists():
        print(f"error: source dir not found: {source_dir}", file=sys.stderr)
        return 0

    today = date.today().isoformat()
    output_dir.mkdir(parents=True, exist_ok=True)

    files = sorted(source_dir.glob("*.c")) + sorted(source_dir.glob("*.i"))
    count = 0

    for src in files:
        # Skip non-files just in case
        if not src.is_file():
            continue

        cname = src.stem  # e.g. "ghost" for ghost.i
        # Sanitize: dashes → underscores (rare in wp_acsl), dots → underscores
        cname_safe = cname.replace("-", "_").replace(".", "_")

        case_id = f"part2_acsl_features/frama_c_wp_acsl/{cname_safe}"
        case_dir = output_dir / cname_safe
        case_dir.mkdir(parents=True, exist_ok=True)

        # Preserve original filename (e.g. .i vs .c) for entry_files
        entry_file = src.name
        (case_dir / entry_file).write_bytes(src.read_bytes())

        features = derive_features(src.name.lower())

        emit_case_toml(
            case_dir=case_dir,
            case_id=case_id,
            name=cname_safe,
            entry_file=entry_file,
            upstream_filename=src.name,
            features=features,
            today=today,
        )

        count += 1
        if count <= 5 or count % 20 == 0:
            print(f"  case {case_id} (features: {features})", file=sys.stderr)

    return count


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_dir)
    print(f"\ningested {n} wp_acsl cases to {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
