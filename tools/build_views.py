#!/usr/bin/env python3
"""Aggregate all case.toml files into top-level manifest + 5 shards + 5 markdown views.

See docs/architecture.md §7.3 + §8 + §17 for design.

Outputs:
    manifest.toml                    — top-level: bench version + 5 shard refs + counts
    manifest_part{1..5}.toml         — per-part case index (key fields cached)
    views/by_part.md                 — cases grouped by part
    views/by_source.md               — cases grouped by source
    views/by_difficulty.md           — cases sorted by difficulty_overall desc
    views/by_feature.md              — cases grouped by feature tag
    views/stats.md                   — summary numbers (counts per part / source / quality / etc.)

Usage:
    build_views.py [--cases-dir DIR] [--check]

    --check  Build outputs in-memory and compare to existing files; exit 1 if any
             diff. Useful in CI to verify manifests are up-to-date.
"""

from __future__ import annotations

import argparse
import sys
import tomllib
from collections import Counter, defaultdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION, BENCH_VERSION  # noqa: E402

PART_NAMES = {
    1: "part1_c_features",
    2: "part2_acsl_features",
    3: "part3_curated",
    4: "part4_real_targets",
    5: "part5_mined",
}

PART_TITLES = {
    1: "Part 1 — C 语言特性",
    2: "Part 2 — ACSL 语言特性",
    3: "Part 3 — 短小算法 (curated)",
    4: "Part 4 — 真实验证目标项目",
    5: "Part 5 — mined 外部 corpus",
}


# -----------------------------------------------------------
# TOML serialization helpers
# -----------------------------------------------------------

def _toml_string(s: str) -> str:
    r"""Render a Python string as a TOML basic string literal."""
    out = ['"']
    for ch in s:
        cp = ord(ch)
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif cp < 0x20:
            out.append(
                {"\b": "\\b", "\t": "\\t", "\n": "\\n",
                 "\f": "\\f", "\r": "\\r"}.get(ch, f"\\u{cp:04x}")
            )
        else:
            out.append(ch)
    out.append('"')
    return "".join(out)


def _toml_list_str(xs: list[str]) -> str:
    return "[" + ", ".join(_toml_string(x) for x in xs) + "]"


# -----------------------------------------------------------
# Load case data
# -----------------------------------------------------------

def _load_one_case(case_toml: Path) -> dict[str, Any]:
    """Load a single case.toml and extract the cached-shard fields + part."""
    try:
        data = tomllib.loads(case_toml.read_text())
    except Exception as e:
        raise RuntimeError(f"failed to parse {case_toml}: {e}")

    prov = data.get("provenance", {})
    diff = data.get("difficulty", {})
    var = data.get("variants", {})

    return {
        "id": data.get("id", ""),
        "path": str(case_toml.parent.relative_to(REPO_ROOT).as_posix()),
        "part": data.get("part", 0),
        "name": data.get("name", ""),
        "source": prov.get("source", ""),
        "license": prov.get("license", ""),
        "quality": prov.get("quality", ""),
        "intent": prov.get("intent", ""),
        "difficulty_overall": diff.get("overall", 0),
        "features": list(data.get("features", [])),
        "has_ground_truth": bool(var.get("ground_truth", "")),
        "has_stripped": bool(var.get("stripped", "")),
    }


def load_cases(cases_dir: Path) -> dict[int, list[dict]]:
    by_part: dict[int, list[dict]] = {p: [] for p in PART_NAMES}
    if not cases_dir.exists():
        return by_part
    for case_toml in sorted(cases_dir.rglob("case.toml")):
        c = _load_one_case(case_toml)
        p = c["part"]
        if p in by_part:
            by_part[p].append(c)
    for p in by_part:
        by_part[p].sort(key=lambda c: c["id"])
    return by_part


# -----------------------------------------------------------
# Top-level manifest.toml
# -----------------------------------------------------------

def render_top_manifest(by_part: dict[int, list[dict]], generated_at: str) -> str:
    lines = [
        f'schema_version = "{SCHEMA_VERSION}"',
        f'bench_version  = "{BENCH_VERSION}"',
        f'generated_at   = "{generated_at}"',
        "",
    ]
    for p in sorted(PART_NAMES):
        name = PART_NAMES[p]
        n = len(by_part[p])
        lines += [
            f"[parts.{name}]",
            f'manifest   = "manifest_part{p}.toml"',
            f"case_count = {n}",
            "",
        ]
    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# Per-part shard manifest_partN.toml
# -----------------------------------------------------------

def render_shard(part: int, cases: list[dict], generated_at: str) -> str:
    lines = [
        f'schema_version = "{SCHEMA_VERSION}"',
        f"part           = {part}",
        f'part_name      = "{PART_NAMES[part]}"',
        f'generated_at   = "{generated_at}"',
        f"case_count     = {len(cases)}",
        "",
    ]
    for c in cases:
        lines += [
            "[[case]]",
            f"id                 = {_toml_string(c['id'])}",
            f"path               = {_toml_string(c['path'])}",
            f"source             = {_toml_string(c['source'])}",
            f"license            = {_toml_string(c['license'])}",
            f"quality            = {_toml_string(c['quality'])}",
            f"intent             = {_toml_string(c['intent'])}",
            f"difficulty_overall = {c['difficulty_overall']}",
            f"features           = {_toml_list_str(c['features'])}",
            f"has_ground_truth   = {str(c['has_ground_truth']).lower()}",
            f"has_stripped       = {str(c['has_stripped']).lower()}",
            "",
        ]
    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# View: by_part.md
# -----------------------------------------------------------

def render_view_by_part(by_part: dict[int, list[dict]], generated_at: str) -> str:
    lines = [
        "# Bench Cases — by Part",
        "",
        f"> Generated: {generated_at}",
        f"> Bench version: {BENCH_VERSION}",
        "",
    ]
    for p in sorted(PART_NAMES):
        cases = by_part[p]
        lines.append(f"## {PART_TITLES[p]} ({len(cases)} case(s))")
        lines.append("")
        if not cases:
            lines.append("_(empty)_")
            lines.append("")
            continue
        lines.append("| id | name | source | difficulty | features |")
        lines.append("|---|---|---|---|---|")
        for c in cases:
            feat = ", ".join(c["features"]) or "—"
            lines.append(
                f"| `{c['id']}` | {c['name']} | {c['source']} | "
                f"{c['difficulty_overall']} | {feat} |"
            )
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# View: by_source.md
# -----------------------------------------------------------

def render_view_by_source(by_part: dict[int, list[dict]], generated_at: str) -> str:
    all_cases = [c for cs in by_part.values() for c in cs]
    by_src: dict[str, list[dict]] = defaultdict(list)
    for c in all_cases:
        by_src[c["source"]].append(c)
    lines = [
        "# Bench Cases — by Source",
        "",
        f"> Generated: {generated_at}",
        f"> Total: {len(all_cases)} case(s) across {len(by_src)} source(s)",
        "",
    ]
    for src in sorted(by_src):
        cases = sorted(by_src[src], key=lambda c: c["id"])
        lines.append(f"## `{src}` ({len(cases)} case(s))")
        lines.append("")
        lines.append("| id | name | part | difficulty | license |")
        lines.append("|---|---|---|---|---|")
        for c in cases:
            lines.append(
                f"| `{c['id']}` | {c['name']} | {c['part']} | "
                f"{c['difficulty_overall']} | {c['license']} |"
            )
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# View: by_difficulty.md
# -----------------------------------------------------------

def render_view_by_difficulty(by_part: dict[int, list[dict]], generated_at: str) -> str:
    all_cases = [c for cs in by_part.values() for c in cs]
    by_diff: dict[int, list[dict]] = defaultdict(list)
    for c in all_cases:
        by_diff[c["difficulty_overall"]].append(c)
    lines = [
        "# Bench Cases — by Difficulty (overall)",
        "",
        f"> Generated: {generated_at}",
        f"> Total: {len(all_cases)} case(s)",
        "",
        "标度锚点：docs/manifest-schema.md §7.3",
        "",
    ]
    for d in sorted(by_diff.keys(), reverse=True):
        cases = sorted(by_diff[d], key=lambda c: c["id"])
        lines.append(f"## Difficulty {d} ({len(cases)} case(s))")
        lines.append("")
        lines.append("| id | name | source | features |")
        lines.append("|---|---|---|---|")
        for c in cases:
            feat = ", ".join(c["features"]) or "—"
            lines.append(f"| `{c['id']}` | {c['name']} | {c['source']} | {feat} |")
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# View: by_feature.md
# -----------------------------------------------------------

def render_view_by_feature(by_part: dict[int, list[dict]], generated_at: str) -> str:
    all_cases = [c for cs in by_part.values() for c in cs]
    by_feat: dict[str, list[dict]] = defaultdict(list)
    for c in all_cases:
        for f in c["features"]:
            by_feat[f].append(c)
    lines = [
        "# Bench Cases — by Feature",
        "",
        f"> Generated: {generated_at}",
        f"> {len(by_feat)} distinct feature(s) tagged across {len(all_cases)} case(s)",
        "",
    ]
    if not by_feat:
        lines.append("_(no features tagged yet)_")
        lines.append("")
    for f in sorted(by_feat):
        cases = sorted(by_feat[f], key=lambda c: c["id"])
        lines.append(f"## `{f}` ({len(cases)} case(s))")
        lines.append("")
        for c in cases:
            lines.append(f"- `{c['id']}` — {c['name']} (part {c['part']}, difficulty {c['difficulty_overall']})")
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# View: stats.md
# -----------------------------------------------------------

def render_view_stats(by_part: dict[int, list[dict]], generated_at: str) -> str:
    all_cases = [c for cs in by_part.values() for c in cs]
    total = len(all_cases)
    lines = [
        "# Bench Statistics",
        "",
        f"> Generated: {generated_at}",
        f"> Bench version: {BENCH_VERSION} (schema: {SCHEMA_VERSION})",
        "",
        f"**Total cases**: {total}",
        "",
        "## By Part",
        "",
        "| Part | Name | Count |",
        "|---|---|---|",
    ]
    for p in sorted(PART_NAMES):
        lines.append(f"| {p} | {PART_NAMES[p]} | {len(by_part[p])} |")
    lines.append("")

    def _section(title: str, counts: Counter):
        lines.append(f"## {title}")
        lines.append("")
        if not counts:
            lines.append("_(empty)_")
            lines.append("")
            return
        lines.append("| Value | Count |")
        lines.append("|---|---|")
        for k, v in counts.most_common():
            lines.append(f"| `{k}` | {v} |")
        lines.append("")

    _section("By Source", Counter(c["source"] for c in all_cases))
    _section("By Quality", Counter(c["quality"] for c in all_cases))
    _section("By Intent", Counter(c["intent"] for c in all_cases))
    _section("By License", Counter(c["license"] for c in all_cases))

    # Difficulty distribution
    diff_counts = Counter(c["difficulty_overall"] for c in all_cases)
    lines.append("## Difficulty Distribution (overall)")
    lines.append("")
    lines.append("| Level | Count |")
    lines.append("|---|---|")
    for d in range(6):
        lines.append(f"| {d} | {diff_counts.get(d, 0)} |")
    lines.append("")

    # Top features
    feat_counts = Counter(f for c in all_cases for f in c["features"])
    lines.append(f"## Feature Tags ({len(feat_counts)} distinct)")
    lines.append("")
    if not feat_counts:
        lines.append("_(none tagged yet)_")
    else:
        lines.append("| Feature | Count |")
        lines.append("|---|---|")
        for f, n in feat_counts.most_common():
            lines.append(f"| `{f}` | {n} |")
    lines.append("")

    # Variants
    has_gt = sum(1 for c in all_cases if c["has_ground_truth"])
    has_str = sum(1 for c in all_cases if c["has_stripped"])
    lines.append("## Variants Coverage")
    lines.append("")
    lines.append(f"- Cases with `ground_truth`: {has_gt} / {total}")
    lines.append(f"- Cases with `stripped`:     {has_str} / {total}")
    lines.append("")

    return "\n".join(lines).rstrip() + "\n"


# -----------------------------------------------------------
# Build orchestrator
# -----------------------------------------------------------

def build_all(cases_dir: Path) -> dict[Path, str]:
    """Return mapping {target_path -> contents} for all generated files."""
    by_part = load_cases(cases_dir)
    generated_at = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")

    out: dict[Path, str] = {
        REPO_ROOT / "manifest.toml": render_top_manifest(by_part, generated_at),
    }
    for p in PART_NAMES:
        out[REPO_ROOT / f"manifest_part{p}.toml"] = render_shard(p, by_part[p], generated_at)

    views_dir = REPO_ROOT / "views"
    out[views_dir / "by_part.md"] = render_view_by_part(by_part, generated_at)
    out[views_dir / "by_source.md"] = render_view_by_source(by_part, generated_at)
    out[views_dir / "by_difficulty.md"] = render_view_by_difficulty(by_part, generated_at)
    out[views_dir / "by_feature.md"] = render_view_by_feature(by_part, generated_at)
    out[views_dir / "stats.md"] = render_view_stats(by_part, generated_at)
    return out


def write_outputs(outputs: dict[Path, str]) -> None:
    for path, content in outputs.items():
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content)


def diff_outputs(outputs: dict[Path, str]) -> list[Path]:
    """Return list of paths where existing file differs from expected content.

    Note: `generated_at` differs every run, so we ignore lines starting with
    'generated_at' when comparing.
    """
    diffs = []

    def _normalize(text: str) -> str:
        return "\n".join(
            ln for ln in text.splitlines()
            if not ln.lstrip().startswith("generated_at")
            and not ln.lstrip().startswith("> Generated:")
        )

    for path, expected in outputs.items():
        if not path.exists():
            diffs.append(path)
            continue
        actual = path.read_text()
        if _normalize(actual) != _normalize(expected):
            diffs.append(path)
    return diffs


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--cases-dir", type=Path, default=REPO_ROOT / "cases")
    p.add_argument("--check", action="store_true",
                   help="Don't write; compare current outputs to existing files. "
                        "Exit 1 if any mismatch (ignoring timestamp lines).")
    args = p.parse_args()

    outputs = build_all(args.cases_dir)

    if args.check:
        diffs = diff_outputs(outputs)
        if diffs:
            print("OUT-OF-DATE manifests/views:", file=sys.stderr)
            for p in diffs:
                print(f"  {p.relative_to(REPO_ROOT)}", file=sys.stderr)
            print("\nRun: python3 tools/build_views.py (no --check) to regenerate.",
                  file=sys.stderr)
            sys.exit(1)
        print("manifests + views are up-to-date.", file=sys.stderr)
        sys.exit(0)

    write_outputs(outputs)
    n_cases = sum(int(p.name.startswith("manifest_part") and ".toml" in p.name) for p in outputs)
    total_cases = sum(
        len([c for c in load_cases(args.cases_dir).get(p, [])]) for p in PART_NAMES
    )
    print(f"wrote {len(outputs)} files: 1 top manifest, {n_cases} shards, "
          f"5 views (covering {total_cases} cases)", file=sys.stderr)


if __name__ == "__main__":
    main()
