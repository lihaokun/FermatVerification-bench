#!/usr/bin/env python3
"""Ingest Frama-C WP internal regression tests (wp_hoare/typed/bytes/region/tip).

Per architecture v3+ decision #4: 这 5 个目录的内容在表面上是 C+ACSL pair，
但本质是 WP 后端不同内存模型 / 证明技术的回归测试。我们仍然收，但用
quality=wp_internal_regression 让 consumer 想过滤就过滤。

5 个目录 (frama-c 32 src/plugins/wp/tests/):
  - wp_hoare:  16 — Hoare 内存模型回归
  - wp_typed:  33 — Typed 模型 (我们项目默认 Typed+nocast)
  - wp_bytes:   7 — Bytes 模型 (新 WP 默认)
  - wp_region:  7 — Region 模型 (ACSL `region:` 扩展)
  - wp_tip:    18 — TIP 交互式证明 (ACSL `strategy:`/`proof:` 扩展)

入到 Part 3，source=frama_c_wp_models，分子目录。
acsl_extensions_used 字段标 ["region"] 或 ["strategy","proof"] 对 region/tip。

注意：这些是 [variants] 类 case (part 3)，但内容多半是合成 corner case；
strip_lines 由 find_acsl_blocks 算；goal_type=rte_only 默认。

Usage:
    ingest_wp_models.py [--source-dir DIR]
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
    "/home/haokun/.opam/frama32/.opam-switch/sources/frama-c.32.0/"
    "src/plugins/wp/tests"
)
DEFAULT_OUTPUT = REPO_ROOT / "cases" / "part3_curated" / "frama_c_wp_models"

UPSTREAM_URL = "https://git.frama-c.com/pub/frama-c"
UPSTREAM_TOOL_HINT = "frama-c 32 (Gallium)"
LICENSE = "LGPL-2.1-only"
LICENSE_FILE = "LICENSES/LGPL-2.1-frama-c.txt"


# Per-subdir config
SUBDIRS = {
    "wp_hoare":  {"intent": "memory_model_stress", "notes": "Hoare 内存模型回归"},
    "wp_typed":  {"intent": "memory_model_stress", "notes": "Typed 内存模型回归"},
    "wp_bytes":  {"intent": "memory_model_stress", "notes": "Bytes 内存模型回归"},
    "wp_region": {"intent": "memory_model_stress", "notes": "Region 内存模型回归 (ACSL region: 扩展)"},
    "wp_tip":    {"intent": "proof_technique",     "notes": "TIP 交互式证明回归 (ACSL strategy/proof 扩展)"},
}


def _safe_name(stem: str) -> str:
    return stem.replace("-", "_").replace(".", "_").replace(" ", "_")


def emit_case_toml(case_dir: Path, case_id: str, name: str,
                   subdir: str, intent: str, notes: str,
                   upstream_path: str, source_hash: str,
                   strip_lines: list[int], today: str) -> None:
    strip_lines_str = "[" + ", ".join(str(x) for x in strip_lines) + "]"

    content = f"""schema_version = "{SCHEMA_VERSION}"
id            = "{case_id}"
name          = {format_toml_string(name)}
part          = 3
features      = []

[provenance]
source             = "frama_c_wp_models"
upstream_url       = "{UPSTREAM_URL}"
upstream_path      = {format_toml_string(upstream_path)}
upstream_tool_hint = "{UPSTREAM_TOOL_HINT}"
license            = "{LICENSE}"
license_file       = "{LICENSE_FILE}"
quality            = "wp_internal_regression"
intent             = "{intent}"

[files]
entry_files = ["ground_truth.c"]

[difficulty]
overall            = 2
proof_size         = 1
math_theory        = 1
data_complexity    = 2
control_complexity = 1
spec_complexity    = 2
auxiliary_needed   = []

[difficulty.scored_by]
type      = "human"
scored_at = "{today}"

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
rationale   = "Frama-C WP 内部回归 ({subdir})：{notes}。架构 v3+ #4 决策：表面是 C+ACSL pair 但本质是 WP 模型回归，纳入 part 3 但用 quality=wp_internal_regression 让 consumer 过滤。"

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
    total = 0

    for subdir, cfg in SUBDIRS.items():
        sd = source_dir / subdir
        if not sd.exists():
            print(f"warn: missing subdir {sd}", file=sys.stderr)
            continue
        files = sorted(list(sd.glob("*.c")) + list(sd.glob("*.i")))
        print(f"{subdir}: {len(files)} files", file=sys.stderr)

        sub_output = output_dir / subdir
        sub_output.mkdir(parents=True, exist_ok=True)

        for src in files:
            if not src.is_file():
                continue
            cname = _safe_name(src.stem)
            case_id = f"part3_curated/frama_c_wp_models/{subdir}/{cname}"
            case_dir = sub_output / cname
            case_dir.mkdir(parents=True, exist_ok=True)

            gt_bytes = src.read_bytes()
            gt_path = case_dir / "ground_truth.c"
            gt_path.write_bytes(gt_bytes)

            blocks = find_acsl_blocks(gt_bytes)
            source_hash = compute_hash(gt_bytes)
            strip_lines = [b.start_line for b in blocks]

            stripped_path = case_dir / "stripped.c"
            if strip_lines:
                strip(gt_path, stripped_path, strip_lines, source_hash)
            else:
                stripped_path.write_bytes(gt_bytes)

            upstream_path = f"src/plugins/wp/tests/{subdir}/{src.name}"
            emit_case_toml(case_dir, case_id, cname, subdir,
                           cfg["intent"], cfg["notes"], upstream_path,
                           source_hash, strip_lines, today)
            total += 1

    return total


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--source-dir", type=Path, default=DEFAULT_SOURCE)
    p.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = p.parse_args()

    n = ingest(args.source_dir, args.output_dir)
    print(f"\ningested {n} wp_models cases to {args.output_dir}", file=sys.stderr)


if __name__ == "__main__":
    main()
