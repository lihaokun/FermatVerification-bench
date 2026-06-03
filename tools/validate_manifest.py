#!/usr/bin/env python3
"""Validate all case.toml files against the bench manifest schema.

See docs/manifest-schema.md §16 for the rules implemented here.

Usage:
    validate_manifest.py [--cases-dir DIR] [--check-files] [--check-strip-hash]

Exit codes:
    0 — no errors (warnings allowed)
    1 — at least one error found
    2 — internal/invocation error

Default mode runs §16.1 (field-level) + §16.2 (cross-field) checks.
    --check-files       additionally runs §16.3 (file existence)
    --check-strip-hash  additionally runs §16.4 (strip_policy.source_hash matches ground_truth content)
"""

from __future__ import annotations

import argparse
import re
import sys
import tomllib
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from schema_version import SCHEMA_VERSION  # noqa: E402
from lib import vocab  # noqa: E402
from strip_acsl import compute_hash  # noqa: E402

ISO_DATE_RE = re.compile(r"^\d{4}-\d{2}-\d{2}$")
SPDX_LICENSE_REF_RE = re.compile(r"^LicenseRef-[A-Za-z0-9.\-_]+$")
SPDX_TOKEN_RE = re.compile(r"^[A-Za-z0-9.\-+]+$")  # 简单 SPDX id token (e.g., MIT, GPL-2.0-only)


# -----------------------------------------------------------
# Data structures
# -----------------------------------------------------------

@dataclass
class Violation:
    case_id: str       # 相对 cases/ 的路径，或 "<schema>" 等特殊标记
    level: str         # "error" | "warning"
    field: str         # 字段路径，如 "provenance.license"
    message: str


@dataclass
class CheckOptions:
    check_files: bool = False
    check_strip_hash: bool = False


# -----------------------------------------------------------
# License expression validation (best-effort without library)
# -----------------------------------------------------------

def _is_valid_license_expr(expr: str) -> bool:
    """Best-effort SPDX expression validator (without external library).

    Accepts:
        - "NOASSERTION"
        - "LicenseRef-..."
        - Single SPDX id token
        - "A OR B" / "A AND B" / "A WITH B" 简单组合（不递归校验括号嵌套）
    """
    if not expr:
        return False
    s = expr.strip()
    if s == "NOASSERTION":
        return True
    if SPDX_LICENSE_REF_RE.match(s):
        return True

    # 移除括号 + 拆分 OR / AND / WITH
    cleaned = s.replace("(", " ").replace(")", " ")
    tokens = re.split(r"\s+(?:OR|AND|WITH)\s+", cleaned)
    for tok in tokens:
        tok = tok.strip()
        if not tok:
            return False
        if SPDX_LICENSE_REF_RE.match(tok):
            continue
        if not SPDX_TOKEN_RE.match(tok):
            return False
    return True


# -----------------------------------------------------------
# Helpers
# -----------------------------------------------------------

def _require(d: dict, key: str, ty: type | tuple[type, ...]) -> tuple[bool, Any]:
    if key not in d:
        return False, None
    v = d[key]
    if not isinstance(v, ty):
        return False, v
    return True, v


def _check_int_0_5(d: dict, key: str, case_id: str, prefix: str,
                   vio: list[Violation]) -> None:
    if key not in d:
        vio.append(Violation(case_id, "error", f"{prefix}.{key}", "missing required field"))
        return
    v = d[key]
    if not isinstance(v, int) or isinstance(v, bool):
        vio.append(Violation(case_id, "error", f"{prefix}.{key}",
                             f"expected int 0–5, got {type(v).__name__}"))
        return
    if not (0 <= v <= 5):
        vio.append(Violation(case_id, "error", f"{prefix}.{key}",
                             f"value {v} out of range 0–5"))


def _check_list_in_vocab(d: dict, key: str, vocab_set: frozenset, case_id: str,
                         prefix: str, vio: list[Violation],
                         required: bool = True, min_size: int = 0) -> None:
    if key not in d:
        if required:
            vio.append(Violation(case_id, "error", f"{prefix}.{key}", "missing required field"))
        return
    v = d[key]
    if not isinstance(v, list):
        vio.append(Violation(case_id, "error", f"{prefix}.{key}",
                             f"expected list, got {type(v).__name__}"))
        return
    if len(v) < min_size:
        vio.append(Violation(case_id, "error", f"{prefix}.{key}",
                             f"requires at least {min_size} element(s), got {len(v)}"))
    for i, x in enumerate(v):
        if not isinstance(x, str):
            vio.append(Violation(case_id, "error", f"{prefix}.{key}[{i}]",
                                 f"expected str, got {type(x).__name__}"))
            continue
        if x not in vocab_set:
            vio.append(Violation(case_id, "error", f"{prefix}.{key}[{i}]",
                                 f"value '{x}' not in vocabulary"))


def _check_iso_date(d: dict, key: str, case_id: str, prefix: str,
                    vio: list[Violation]) -> None:
    if key not in d:
        vio.append(Violation(case_id, "error", f"{prefix}.{key}", "missing required field"))
        return
    v = d[key]
    if not isinstance(v, str) or not ISO_DATE_RE.match(v):
        vio.append(Violation(case_id, "error", f"{prefix}.{key}",
                             f"expected ISO date (YYYY-MM-DD), got {v!r}"))


def _check_required_str(d: dict, key: str, case_id: str, prefix: str,
                        vio: list[Violation], allow_empty: bool = True) -> bool:
    if key not in d:
        vio.append(Violation(case_id, "error", f"{prefix}.{key}", "missing required field"))
        return False
    v = d[key]
    if not isinstance(v, str):
        vio.append(Violation(case_id, "error", f"{prefix}.{key}",
                             f"expected str, got {type(v).__name__}"))
        return False
    if not allow_empty and not v:
        vio.append(Violation(case_id, "error", f"{prefix}.{key}", "must be non-empty"))
        return False
    return True


# -----------------------------------------------------------
# Section checkers (§16.1 + §16.2)
# -----------------------------------------------------------

def check_identity(data: dict, case_dir: Path, case_id: str, vio: list[Violation]) -> int | None:
    """Return part number if valid, else None."""
    # schema_version
    if "schema_version" not in data:
        vio.append(Violation(case_id, "error", "schema_version", "missing"))
    elif data["schema_version"] != SCHEMA_VERSION:
        vio.append(Violation(
            case_id, "error", "schema_version",
            f"got {data['schema_version']!r}, expected {SCHEMA_VERSION!r}"))

    # id  (= 相对 cases/ 的路径)
    if "id" not in data:
        vio.append(Violation(case_id, "error", "id", "missing"))
    else:
        rel = case_dir.relative_to(REPO_ROOT / "cases").as_posix()
        if data["id"] != rel:
            vio.append(Violation(
                case_id, "error", "id",
                f"got {data['id']!r}, expected directory path {rel!r}"))

    # name
    _check_required_str(data, "name", case_id, "", vio, allow_empty=False)

    # part
    if "part" not in data:
        vio.append(Violation(case_id, "error", "part", "missing"))
        return None
    p = data["part"]
    if not isinstance(p, int) or isinstance(p, bool) or p not in (1, 2, 3, 4, 5):
        vio.append(Violation(case_id, "error", "part", f"expected int in 1..5, got {p!r}"))
        return None

    # part 与 id 前缀一致
    if "id" in data and isinstance(data["id"], str):
        expected_prefix = vocab.PART_DIR_PREFIX[p] + "/"
        if not data["id"].startswith(expected_prefix):
            vio.append(Violation(
                case_id, "error", "part",
                f"part={p} but id does not start with {expected_prefix!r}"))
    return p


def check_provenance(data: dict, case_id: str, vio: list[Violation]) -> None:
    if "provenance" not in data:
        vio.append(Violation(case_id, "error", "provenance", "missing required section"))
        return
    prov = data["provenance"]
    prefix = "provenance"

    # source
    if _check_required_str(prov, "source", case_id, prefix, vio, allow_empty=False):
        if prov["source"] not in vocab.SOURCE:
            vio.append(Violation(case_id, "error", f"{prefix}.source",
                                 f"value {prov['source']!r} not in vocabulary"))

    # urls / paths
    _check_required_str(prov, "upstream_url", case_id, prefix, vio, allow_empty=False)
    _check_required_str(prov, "upstream_path", case_id, prefix, vio, allow_empty=True)
    # upstream_commit / upstream_tool_hint optional

    # license — SPDX expression
    if _check_required_str(prov, "license", case_id, prefix, vio, allow_empty=False):
        if not _is_valid_license_expr(prov["license"]):
            vio.append(Violation(case_id, "warning", f"{prefix}.license",
                                 f"value {prov['license']!r} doesn't look like a valid SPDX expression"))

    _check_required_str(prov, "license_file", case_id, prefix, vio, allow_empty=False)

    # quality / intent
    if _check_required_str(prov, "quality", case_id, prefix, vio, allow_empty=False):
        if prov["quality"] not in vocab.QUALITY:
            vio.append(Violation(case_id, "error", f"{prefix}.quality",
                                 f"value {prov['quality']!r} not in vocabulary"))
    if _check_required_str(prov, "intent", case_id, prefix, vio, allow_empty=False):
        if prov["intent"] not in vocab.INTENT:
            vio.append(Violation(case_id, "error", f"{prefix}.intent",
                                 f"value {prov['intent']!r} not in vocabulary"))


def check_files(data: dict, case_id: str, vio: list[Violation]) -> None:
    if "files" not in data:
        vio.append(Violation(case_id, "error", "files", "missing required section"))
        return
    files = data["files"]
    if "entry_files" not in files:
        vio.append(Violation(case_id, "error", "files.entry_files", "missing"))
    elif not isinstance(files["entry_files"], list):
        vio.append(Violation(case_id, "error", "files.entry_files", "must be list"))
    elif not all(isinstance(x, str) for x in files["entry_files"]):
        vio.append(Violation(case_id, "error", "files.entry_files", "all elements must be str"))
    elif not files["entry_files"]:
        vio.append(Violation(case_id, "warning", "files.entry_files", "empty list (case has no entry files?)"))

    if "include_dirs" in files and not isinstance(files["include_dirs"], list):
        vio.append(Violation(case_id, "error", "files.include_dirs", "must be list if present"))


def check_features(data: dict, case_id: str, vio: list[Violation]) -> None:
    _check_list_in_vocab(data, "features", vocab.FEATURES, case_id, "", vio,
                         required=True, min_size=0)


def check_difficulty(data: dict, case_id: str, vio: list[Violation]) -> None:
    if "difficulty" not in data:
        vio.append(Violation(case_id, "error", "difficulty", "missing required section"))
        return
    diff = data["difficulty"]
    for k in ("overall", "proof_size", "math_theory", "data_complexity",
              "control_complexity", "spec_complexity"):
        _check_int_0_5(diff, k, case_id, "difficulty", vio)
    _check_list_in_vocab(diff, "auxiliary_needed", vocab.AUXILIARY, case_id,
                         "difficulty", vio, required=True, min_size=0)

    # difficulty.scored_by
    if "scored_by" not in diff:
        vio.append(Violation(case_id, "error", "difficulty.scored_by", "missing"))
        return
    sb = diff["scored_by"]
    if _check_required_str(sb, "type", case_id, "difficulty.scored_by", vio):
        if sb["type"] not in vocab.CLASSIFIED_BY_TYPE:
            vio.append(Violation(case_id, "error", "difficulty.scored_by.type",
                                 f"value {sb['type']!r} not in vocabulary"))
        elif sb["type"] == "llm":
            for k in ("model", "prompt_version"):
                if k not in sb or not isinstance(sb[k], str) or not sb[k]:
                    vio.append(Violation(case_id, "error", f"difficulty.scored_by.{k}",
                                         "required when type='llm'"))
    _check_iso_date(sb, "scored_at", case_id, "difficulty.scored_by", vio)


def check_parse(data: dict, case_id: str, part: int, vio: list[Violation]) -> None:
    if part in (1, 2):
        if "parse" not in data:
            vio.append(Violation(case_id, "error", "parse",
                                 f"missing required section (part={part})"))
            return
        parse = data["parse"]
        _check_list_in_vocab(parse, "constructs_used", vocab.CONSTRUCTS, case_id,
                             "parse", vio, required=True, min_size=0)
        if "known_issues" in parse and not isinstance(parse["known_issues"], list):
            vio.append(Violation(case_id, "error", "parse.known_issues", "must be list if present"))
    else:
        if "parse" in data:
            vio.append(Violation(case_id, "error", "parse",
                                 f"section not allowed for part={part}"))


def check_verification(data: dict, case_id: str, part: int, vio: list[Violation]) -> None:
    if part in (3, 4, 5):
        if "verification" not in data:
            vio.append(Violation(case_id, "error", "verification",
                                 f"missing required section (part={part})"))
            return
        verif = data["verification"]
        if "target" not in verif:
            vio.append(Violation(case_id, "error", "verification.target", "missing"))
            return
        _check_list_in_vocab(verif, "target", vocab.TARGET, case_id, "verification", vio,
                             required=True, min_size=1)
        t = verif.get("target", [])
        if isinstance(t, list) and "functional" in t and "total_correctness" in t:
            vio.append(Violation(case_id, "warning", "verification.target",
                                 "'total_correctness' already implies 'functional'; listing both is redundant"))
    else:
        if "verification" in data:
            vio.append(Violation(case_id, "error", "verification",
                                 f"section not allowed for part={part}"))


def check_variants(data: dict, case_id: str, part: int, vio: list[Violation]) -> None:
    if part in (3, 4, 5):
        if "variants" not in data:
            vio.append(Violation(case_id, "error", "variants",
                                 f"missing required section (part={part})"))
            return
        var = data["variants"]
        if "stripped" not in var or not isinstance(var["stripped"], str) or not var["stripped"]:
            vio.append(Violation(case_id, "error", "variants.stripped",
                                 "required non-empty string"))
        if "ground_truth" in var and not isinstance(var["ground_truth"], str):
            vio.append(Violation(case_id, "error", "variants.ground_truth", "must be str if present"))

        # strip_policy
        if "strip_policy" not in var:
            vio.append(Violation(case_id, "error", "variants.strip_policy", "missing"))
            return
        sp = var["strip_policy"]
        if _check_required_str(sp, "goal_type", case_id, "variants.strip_policy", vio):
            if sp["goal_type"] not in vocab.GOAL_TYPE:
                vio.append(Violation(case_id, "error", "variants.strip_policy.goal_type",
                                     f"value {sp['goal_type']!r} not in vocabulary"))
        for k in ("goal_lines", "strip_lines"):
            if k not in sp:
                vio.append(Violation(case_id, "error", f"variants.strip_policy.{k}", "missing"))
            elif not isinstance(sp[k], list):
                vio.append(Violation(case_id, "error", f"variants.strip_policy.{k}",
                                     f"expected list, got {type(sp[k]).__name__}"))
            elif not all(isinstance(x, int) and not isinstance(x, bool) for x in sp[k]):
                vio.append(Violation(case_id, "error", f"variants.strip_policy.{k}",
                                     "all elements must be int"))
        sh = sp.get("source_hash")
        if not isinstance(sh, str) or not (sh.startswith("sha256:") or sh.startswith("merkle:sha256:")):
            vio.append(Violation(case_id, "error", "variants.strip_policy.source_hash",
                                 "must be 'sha256:<hex>' or 'merkle:sha256:<hex>'"))
        _check_required_str(sp, "rationale", case_id, "variants.strip_policy", vio)

        # classified_by
        if "classified_by" not in sp:
            vio.append(Violation(case_id, "error", "variants.strip_policy.classified_by", "missing"))
            return
        cb = sp["classified_by"]
        if _check_required_str(cb, "type", case_id, "variants.strip_policy.classified_by", vio):
            if cb["type"] not in vocab.CLASSIFIED_BY_TYPE:
                vio.append(Violation(case_id, "error", "variants.strip_policy.classified_by.type",
                                     f"value {cb['type']!r} not in vocabulary"))
            elif cb["type"] == "llm":
                for k in ("model", "prompt_version"):
                    if k not in cb or not isinstance(cb[k], str) or not cb[k]:
                        vio.append(Violation(case_id, "error",
                                             f"variants.strip_policy.classified_by.{k}",
                                             "required when type='llm'"))
        if "reviewed_by_human" not in cb or not isinstance(cb["reviewed_by_human"], bool):
            vio.append(Violation(case_id, "error", "variants.strip_policy.classified_by.reviewed_by_human",
                                 "required bool"))
        elif part == 4 and not cb["reviewed_by_human"]:
            vio.append(Violation(case_id, "warning", "variants.strip_policy.classified_by.reviewed_by_human",
                                 "part 4 case: human review strongly recommended (v3 was hard error, v3+ softened to warning)"))
        _check_iso_date(cb, "classified_at", case_id, "variants.strip_policy.classified_by", vio)
    else:
        if "variants" in data:
            vio.append(Violation(case_id, "error", "variants",
                                 f"section not allowed for part={part}"))


def check_project(data: dict, case_id: str, part: int, vio: list[Violation]) -> None:
    if part == 4:
        if "project" not in data:
            vio.append(Violation(case_id, "error", "project",
                                 "missing required section (part=4)"))
            return
        pr = data["project"]
        _check_required_str(pr, "upstream_repo", case_id, "project", vio, allow_empty=False)
        _check_required_str(pr, "upstream_commit", case_id, "project", vio, allow_empty=False)
        if _check_required_str(pr, "build_system", case_id, "project", vio, allow_empty=False):
            if pr["build_system"] not in vocab.BUILD_SYSTEM:
                vio.append(Violation(case_id, "error", "project.build_system",
                                     f"value {pr['build_system']!r} not in vocabulary"))
        for k in ("loc", "function_count"):
            if k not in pr or not isinstance(pr[k], int) or isinstance(pr[k], bool):
                vio.append(Violation(case_id, "error", f"project.{k}", "required int"))
        if _check_required_str(pr, "verification_completeness", case_id, "project", vio):
            if pr["verification_completeness"] not in vocab.VERIFICATION_COMPLETENESS:
                vio.append(Violation(case_id, "error", "project.verification_completeness",
                                     f"value {pr['verification_completeness']!r} not in vocabulary"))
    else:
        if "project" in data:
            vio.append(Violation(case_id, "error", "project",
                                 f"section not allowed for part={part}"))


def check_transformations(data: dict, case_id: str, part: int, vio: list[Violation]) -> None:
    if part == 4:
        if "transformations" not in data:
            vio.append(Violation(case_id, "error", "transformations",
                                 "missing required section (part=4)"))
            return
        tr = data["transformations"]
        if "sequence" not in tr:
            vio.append(Violation(case_id, "error", "transformations.sequence", "missing"))
        elif not isinstance(tr["sequence"], list):
            vio.append(Violation(case_id, "error", "transformations.sequence", "must be list"))
    else:
        if "transformations" in data:
            vio.append(Violation(case_id, "error", "transformations",
                                 f"section not allowed for part={part}"))


def check_mined(data: dict, case_id: str, part: int, vio: list[Violation]) -> None:
    if part == 5:
        if "mined" not in data:
            vio.append(Violation(case_id, "error", "mined",
                                 "missing required section (part=5)"))
            return
        mn = data["mined"]
        if _check_required_str(mn, "source_dataset", case_id, "mined", vio):
            if mn["source_dataset"] not in vocab.SOURCE_DATASET:
                vio.append(Violation(case_id, "error", "mined.source_dataset",
                                     f"value {mn['source_dataset']!r} not in vocabulary"))
            elif mn["source_dataset"] == "casp":
                if "casp" not in mn:
                    vio.append(Violation(case_id, "error", "mined.casp",
                                         "required when source_dataset='casp'"))
                else:
                    c = mn["casp"]
                    if "upstream_verified" not in c or not isinstance(c["upstream_verified"], bool):
                        vio.append(Violation(case_id, "error", "mined.casp.upstream_verified", "required bool"))
                    for k in ("total_goals", "verified_goals"):
                        if k not in c or not isinstance(c[k], int) or isinstance(c[k], bool):
                            vio.append(Violation(case_id, "error", f"mined.casp.{k}", "required int"))
                    if "error_cause" not in c or not isinstance(c["error_cause"], str):
                        vio.append(Violation(case_id, "error", "mined.casp.error_cause",
                                             "required string (may be empty)"))
            elif mn["source_dataset"] == "fm_bench_acsl":
                if "fm_bench" not in mn:
                    vio.append(Violation(case_id, "error", "mined.fm_bench",
                                         "required when source_dataset='fm_bench_acsl'"))
            elif mn["source_dataset"] == "live_fm_bench":
                if "live_fm_bench" not in mn:
                    vio.append(Violation(case_id, "error", "mined.live_fm_bench",
                                         "required when source_dataset='live_fm_bench'"))
                else:
                    sub = mn["live_fm_bench"]
                    for k in ("task", "folder", "file_name", "dataset_source"):
                        _check_required_str(sub, k, case_id, "mined.live_fm_bench", vio)
        _check_required_str(mn, "upstream_id", case_id, "mined", vio)
    else:
        if "mined" in data:
            vio.append(Violation(case_id, "error", "mined",
                                 f"section not allowed for part={part}"))


# -----------------------------------------------------------
# §16.3 file existence
# -----------------------------------------------------------

def check_file_existence(data: dict, case_dir: Path, case_id: str,
                         vio: list[Violation]) -> None:
    files = data.get("files", {})
    for f in files.get("entry_files", []):
        if not isinstance(f, str):
            continue
        p = case_dir / f
        if not p.exists():
            vio.append(Violation(case_id, "error", "files.entry_files",
                                 f"file does not exist: {p.relative_to(REPO_ROOT)}"))

    var = data.get("variants", {})
    if "stripped" in var and isinstance(var["stripped"], str):
        p = case_dir / var["stripped"]
        if not p.exists():
            vio.append(Violation(case_id, "error", "variants.stripped",
                                 f"file does not exist: {p.relative_to(REPO_ROOT)}"))
    if "ground_truth" in var and isinstance(var["ground_truth"], str) and var["ground_truth"]:
        p = case_dir / var["ground_truth"]
        if not p.exists():
            vio.append(Violation(case_id, "error", "variants.ground_truth",
                                 f"file does not exist: {p.relative_to(REPO_ROOT)}"))

    prov = data.get("provenance", {})
    if "license_file" in prov and isinstance(prov["license_file"], str):
        p = REPO_ROOT / prov["license_file"]
        if not p.exists():
            vio.append(Violation(case_id, "error", "provenance.license_file",
                                 f"file does not exist: {prov['license_file']}"))


# -----------------------------------------------------------
# §16.4 strip_policy.source_hash consistency
# -----------------------------------------------------------

def check_strip_hash(data: dict, case_dir: Path, case_id: str,
                     vio: list[Violation]) -> None:
    var = data.get("variants", {})
    sp = var.get("strip_policy", {})
    recorded = sp.get("source_hash")
    gt = var.get("ground_truth")
    if not isinstance(recorded, str) or not isinstance(gt, str) or not gt:
        return
    gt_path = case_dir / gt
    if not gt_path.exists():
        return  # file_existence check will already complain
    if recorded.startswith("merkle:"):
        return  # multi-file merkle root — skipping for now (POC: single-file CASP)
    if gt_path.is_dir():
        vio.append(Violation(case_id, "warning", "variants.strip_policy.source_hash",
                             "ground_truth is a directory; merkle hash not implemented in validator yet"))
        return
    actual = compute_hash(gt_path.read_bytes())
    if actual != recorded:
        vio.append(Violation(case_id, "error", "variants.strip_policy.source_hash",
                             f"hash mismatch: recorded={recorded[:24]}..., actual={actual[:24]}..."))


# -----------------------------------------------------------
# Per-case orchestrator
# -----------------------------------------------------------

def validate_one(case_dir: Path, opts: CheckOptions) -> list[Violation]:
    vio: list[Violation] = []
    try:
        rel = case_dir.relative_to(REPO_ROOT / "cases").as_posix()
    except ValueError:
        rel = case_dir.as_posix()
    case_id = rel

    case_toml = case_dir / "case.toml"
    try:
        data = tomllib.loads(case_toml.read_text())
    except Exception as e:
        vio.append(Violation(case_id, "error", "<file>", f"failed to parse case.toml: {e}"))
        return vio

    part = check_identity(data, case_dir, case_id, vio)
    check_provenance(data, case_id, vio)
    check_files(data, case_id, vio)
    check_features(data, case_id, vio)
    check_difficulty(data, case_id, vio)
    if part is not None:
        check_parse(data, case_id, part, vio)
        check_verification(data, case_id, part, vio)
        check_variants(data, case_id, part, vio)
        check_project(data, case_id, part, vio)
        check_transformations(data, case_id, part, vio)
        check_mined(data, case_id, part, vio)

    if opts.check_files:
        check_file_existence(data, case_dir, case_id, vio)
    if opts.check_strip_hash:
        check_strip_hash(data, case_dir, case_id, vio)

    return vio


def find_cases(cases_dir: Path) -> list[Path]:
    """Find all directories containing a case.toml."""
    if not cases_dir.exists():
        return []
    return sorted(p.parent for p in cases_dir.rglob("case.toml"))


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("--cases-dir", type=Path, default=REPO_ROOT / "cases")
    p.add_argument("--check-files", action="store_true",
                   help="Run §16.3 file existence checks")
    p.add_argument("--check-strip-hash", action="store_true",
                   help="Run §16.4 strip_policy.source_hash consistency check")
    args = p.parse_args()

    opts = CheckOptions(check_files=args.check_files, check_strip_hash=args.check_strip_hash)

    cases = find_cases(args.cases_dir)
    if not cases:
        print(f"no case.toml found under {args.cases_dir}", file=sys.stderr)
        sys.exit(2)

    all_vio: list[Violation] = []
    for case_dir in cases:
        all_vio.extend(validate_one(case_dir, opts))

    errors = [v for v in all_vio if v.level == "error"]
    warnings = [v for v in all_vio if v.level == "warning"]

    # Pretty-print grouped by case
    if all_vio:
        by_case: dict[str, list[Violation]] = {}
        for v in all_vio:
            by_case.setdefault(v.case_id, []).append(v)
        for cid, vs in by_case.items():
            print(f"\n--- {cid} ---")
            for v in vs:
                print(f"  [{v.level.upper():7}] {v.field}: {v.message}")

    print()
    print(f"summary: {len(cases)} case(s) scanned, "
          f"{len(errors)} error(s), {len(warnings)} warning(s)")
    sys.exit(0 if not errors else 1)


if __name__ == "__main__":
    main()
