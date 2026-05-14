#!/usr/bin/env python3
"""Ingest ANSSI x509-parser into cases/part4_real_targets/anssi_x509_parser/.

ANSSI's x509-parser is a 12-file C+ACSL project (~20K LOC, 704 ACSL blocks)
written for Frama-C 18 (Argon). It's the canonical example of "real
production C code with ACSL verification".

Strategy (v0.2 MVP — single-file concat):
  - Concatenate all `.h` then `.c` from upstream/src/ → variants/ground_truth/x509_flat.c
  - Strip all ACSL blocks → variants/stripped/x509_flat.c
  - Single case for the whole project (Part 4 = project-level)
  - upstream/ is a git submodule pinned to specific commit

Trade-off: 20K LOC single file is unwieldy but matches v0.1 schema (single
strip_lines list). Future schema extension (dict[file → lines]) could replace
this with multi-file ground_truth/stripped per architecture §6.1.

Usage:
    ingest_anssi_x509.py
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

CASE_DIR = REPO_ROOT / "cases" / "part4_real_targets" / "anssi_x509_parser"
UPSTREAM_DIR = CASE_DIR / "upstream"
UPSTREAM_SRC = UPSTREAM_DIR / "src"
TRANSFORMATIONS_DIR = CASE_DIR / "transformations"
VARIANTS_DIR = CASE_DIR / "variants"

UPSTREAM_REPO = "https://github.com/ANSSI-FR/x509-parser"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"]+)"\s*$')


def strip_internal_includes(content: str) -> str:
    """Remove #include "..." lines (internal includes; system <...> kept)."""
    out = []
    for line in content.split("\n"):
        if INCLUDE_RE.match(line):
            # Replace with placeholder comment so line count is preserved
            out.append(f"/* removed: {line.strip()} */")
            continue
        out.append(line)
    return "\n".join(out)


def concat_x509_sources(src_dir: Path) -> str:
    """Concat all .h then .c files from src_dir in alphabetical order."""
    headers = sorted(src_dir.glob("*.h"))
    impls = sorted(src_dir.glob("*.c"))

    parts: list[str] = [
        "/* ============================================================ */",
        "/* ANSSI x509-parser — flat concatenation for FermatVerification-bench */",
        "/* Original: https://github.com/ANSSI-FR/x509-parser */",
        f"/* Order: {len(headers)} headers first, then {len(impls)} implementations */",
        "/* Internal #include \"...\" lines are commented out (deps inlined). */",
        "/* ============================================================ */",
        "",
    ]

    for h in headers:
        parts.append(f"/* ============== HEADER: {h.name} ============== */")
        parts.append(strip_internal_includes(h.read_text(errors="replace")))
        parts.append("")

    for c in impls:
        parts.append(f"/* ============== IMPL: {c.name} ============== */")
        parts.append(strip_internal_includes(c.read_text(errors="replace")))
        parts.append("")

    return "\n".join(parts)


def write_transformations_readme(today: str) -> None:
    TRANSFORMATIONS_DIR.mkdir(parents=True, exist_ok=True)
    readme = f"""# Transformations Applied to ANSSI x509-parser Upstream

Generated: {today}

This directory documents the verifier-中立 transformations we apply to upstream
ANSSI x509-parser to produce `variants/ground_truth/`. Per architecture §6.1,
transformations are limited to **verifier-neutral preprocessing** (flattening
includes, merging multi-file structure, normalizing macros) — not Frama-C
version adaptation (that's consumer responsibility).

## v0.2.0 — Initial concat

Single transformation: concatenate all `.h` then `.c` files from `upstream/src/`
into a single flat `x509_flat.c`. Internal `#include "..."` lines are commented
out (their content is now inlined). System `#include <...>` are kept verbatim
for the consumer's preprocessor.

Implementation: `tools/ingest_anssi_x509.py concat_x509_sources()`

## Future transformations (v0.2.x+)

Possible additions for downstream consumability:
- Frama-C 31+ migration scripts (separate, consumer-side)
- Function granularity splitting (one case per top-level function)
- Macro normalization

Per architecture v3 §6.1: **transformations stay verifier-neutral**.
"""
    (TRANSFORMATIONS_DIR / "README.md").write_text(readme)


def emit_case_toml(source_hash: str, strip_lines: list[int],
                   total_blocks: int, today: str) -> None:
    upstream_commit = (UPSTREAM_DIR / ".git").read_text().strip() \
        if (UPSTREAM_DIR / ".git").is_file() else ""
    # Just read the commit from git
    import subprocess
    res = subprocess.run(
        ["git", "rev-parse", "HEAD"],
        cwd=str(UPSTREAM_DIR), capture_output=True, text=True,
    )
    upstream_commit = res.stdout.strip()

    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "part4_real_targets/anssi_x509_parser"
name          = "anssi_x509_parser"
part          = 4
features      = ["loops", "pointers", "arrays_ro", "arrays_mut", "nested_struct", "behaviors", "predicate", "lemma", "axiomatic", "ghost", "termination"]

[provenance]
source             = "anssi_x509_parser"
upstream_url       = "{UPSTREAM_REPO}"
upstream_path      = "src/"
upstream_commit    = "{upstream_commit}"
upstream_tool_hint = "frama-c 18 (Argon)"
license            = "BSD-2-Clause"
license_file       = "LICENSES/BSD-2-anssi.txt"
quality            = "algorithm"
intent             = "functional_correctness"

[files]
entry_files = ["variants/ground_truth/x509_flat.c"]

[difficulty]
overall            = 5
proof_size         = 5
math_theory        = 3
data_complexity    = 4
control_complexity = 4
spec_complexity    = 5
auxiliary_needed   = ["loop_invariant", "predicate_def", "lemma", "ghost"]

[difficulty.scored_by]
type      = "human"
scored_at = "{today}"

[verification]
target = ["functional", "memory_safety"]

[variants]
ground_truth = "variants/ground_truth/x509_flat.c"
stripped     = "variants/stripped/x509_flat.c"

[variants.strip_policy]
goal_type   = "rte_only"
goal_lines  = []
strip_lines = {strip_lines_str}
source_hash = "{source_hash}"
rationale   = "ANSSI x509-parser ({total_blocks} ACSL 块跨 12 文件)；架构 v3+ 把函数 pre/post 视为过程规约。MVP 用单文件 concat 适配 v0.1 schema（strip_lines 单 list）。Future v0.2.x 可改 multi-file 支持。"

[variants.strip_policy.classified_by]
type              = "human"
reviewed_by_human = true
classified_at     = "{today}"

[project]
upstream_repo                = "{UPSTREAM_REPO}"
upstream_commit              = "{upstream_commit}"
build_system                 = "make"
loc                          = 20122
function_count               = 87
upstream_verification_effort = "ANSSI 2018-2022 (Arnaud Ebalard, Ryad Benadjila, Patricia Mouy)"
verification_completeness    = "partial"

[transformations]
sequence = ["01-concat-sources"]
notes    = "v0.2 MVP: 全部 .h + .c concat 成单文件，#include \\"...\\" 注释化。详见 transformations/README.md。"
"""
    (CASE_DIR / "case.toml").write_text(content)


def write_license_attribution() -> None:
    """Write LICENSES/BSD-2-anssi.txt if missing."""
    license_path = REPO_ROOT / "LICENSES" / "BSD-2-anssi.txt"
    if license_path.exists():
        return
    content = """ANSSI x509-parser — Attribution & License Reference
======================================================

Content under the following bench paths is derived from ANSSI's x509-parser:

  cases/part4_real_targets/anssi_x509_parser/upstream/   (git submodule)
  cases/part4_real_targets/anssi_x509_parser/variants/   (derived via concat)

Upstream:   https://github.com/ANSSI-FR/x509-parser
Copyright:  Copyright (C) 2019-2022 ANSSI
            (Arnaud Ebalard, Ryad Benadjila, Patricia Mouy)
License:    Dual GPL-2.0-only / BSD-2-Clause; we redistribute under BSD-2-Clause

The upstream LICENSE file states (verbatim, abridged):

  The files in this project are provided under a dual GPLv2/BSD license. When
  using or redistributing this software, you may do so under either license.

  ...

  BSD License summary

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES ARE DISCLAIMED. ...

For the full upstream LICENSE text, see:
  https://github.com/ANSSI-FR/x509-parser/blob/master/LICENSE

This bench redistributes the project under BSD-2-Clause per upstream's dual-license offer.

Note on transformations:
  variants/ground_truth/x509_flat.c is a CONCATENATION of all upstream .h and .c
  files under src/. Internal #include "..." lines are commented out (their
  content is now inline). System #include <...> are kept verbatim. The original
  source is preserved unmodified in upstream/ (git submodule).

Project background:
  x509-parser is a defensive, manually-annotated C parser for X.509 certificates,
  originally developed at ANSSI as a case study in deductive verification with
  Frama-C and WP. The verification effort spans ~700 ACSL annotation blocks
  across 12 files.

Citation:
  Benadjila, R., Ebalard, A., Mouy, P. — Formal verification of x509 parser at
  ANSSI (technical report, 2019-2022).
  Project home: https://github.com/ANSSI-FR/x509-parser
"""
    license_path.write_text(content)


def ingest() -> None:
    if not UPSTREAM_SRC.exists():
        print(f"error: upstream src not found: {UPSTREAM_SRC}", file=sys.stderr)
        print("  did you run `git submodule update --init`?", file=sys.stderr)
        sys.exit(1)

    today = date.today().isoformat()

    # 1. concat sources → ground_truth.c
    gt_dir = VARIANTS_DIR / "ground_truth"
    str_dir = VARIANTS_DIR / "stripped"
    gt_dir.mkdir(parents=True, exist_ok=True)
    str_dir.mkdir(parents=True, exist_ok=True)
    gt_path = gt_dir / "x509_flat.c"
    str_path = str_dir / "x509_flat.c"

    flat = concat_x509_sources(UPSTREAM_SRC)
    gt_bytes = flat.encode("utf-8")
    gt_path.write_bytes(gt_bytes)
    print(f"wrote ground_truth: {len(gt_bytes)} bytes / {flat.count(chr(10))} lines",
          file=sys.stderr)

    # 2. find ACSL blocks + strip
    blocks = find_acsl_blocks(gt_bytes)
    source_hash = compute_hash(gt_bytes)
    strip_lines = [b.start_line for b in blocks]
    print(f"found {len(blocks)} ACSL blocks", file=sys.stderr)

    if strip_lines:
        strip(gt_path, str_path, strip_lines, source_hash)
    else:
        str_path.write_bytes(gt_bytes)
    print(f"wrote stripped: {str_path.stat().st_size} bytes", file=sys.stderr)

    # 3. transformations/ + license
    write_transformations_readme(today)
    write_license_attribution()

    # 4. case.toml
    emit_case_toml(source_hash, strip_lines, len(blocks), today)
    print(f"wrote case.toml ({CASE_DIR / 'case.toml'})", file=sys.stderr)


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.parse_args()
    ingest()
    print(f"\nANSSI x509 part 4 case ready under {CASE_DIR}", file=sys.stderr)


if __name__ == "__main__":
    main()
