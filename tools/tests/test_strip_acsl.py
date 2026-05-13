"""Tests for tools/strip_acsl.py — A1 POC verification.

Covers the 4 criteria from architecture.md §12.0 A1:
  1. C part byte-faithful after strip
  2. Idempotence (same input + same params → same output)
  3. String literal '/*@' not falsely detected
  4. Hash mismatch → error, no output produced
"""

from __future__ import annotations

import hashlib
import sys
import tomllib
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from strip_acsl import find_acsl_blocks, compute_hash, strip  # noqa: E402

FIXTURES = Path(__file__).parent / "fixtures"
WP_GALLERY = FIXTURES / "wp_gallery"
EDGE = FIXTURES / "edge_cases"


def _load_expected(toml_path: Path) -> dict:
    return tomllib.loads(toml_path.read_text())


def _fixture_files() -> list[tuple[Path, Path]]:
    """Return list of (source, expected_toml) pairs."""
    pairs = []
    for src in sorted(WP_GALLERY.glob("*.c")) + sorted(WP_GALLERY.glob("*.i")):
        exp = src.with_suffix("").with_suffix(".expected.toml")
        if exp.exists():
            pairs.append((src, exp))
    for src in sorted(EDGE.glob("*.c")):
        exp = src.with_suffix("").with_suffix(".expected.toml")
        if exp.exists():
            pairs.append((src, exp))
    return pairs


@pytest.fixture(params=_fixture_files(), ids=lambda p: p[0].name)
def fixture(request):
    src, exp = request.param
    return {
        "src": src,
        "content": src.read_bytes(),
        "expected": _load_expected(exp),
    }


# -----------------------------------------------------------
# Scanner correctness
# -----------------------------------------------------------

def test_scanner_finds_expected_blocks(fixture):
    blocks = find_acsl_blocks(fixture["content"])
    actual_lines = [b.start_line for b in blocks]
    assert actual_lines == fixture["expected"]["all_block_lines"]


def test_scanner_block_vs_line_classification(fixture):
    blocks = find_acsl_blocks(fixture["content"])
    block_lines = [b.start_line for b in blocks if not b.is_line_block]
    line_lines = [b.start_line for b in blocks if b.is_line_block]
    assert block_lines == fixture["expected"]["block_comment_lines"]
    assert line_lines == fixture["expected"]["line_comment_lines"]


def test_hash_matches_expected(fixture):
    actual = compute_hash(fixture["content"])
    assert actual == fixture["expected"]["input_hash"]


# -----------------------------------------------------------
# Edge case: false positives must not be detected
# -----------------------------------------------------------

def test_edge_case_no_false_positives():
    """String literals and char literals containing /*@ or //@ must NOT
    be detected as ACSL blocks."""
    src = EDGE / "string_literal_with_acsl.c"
    exp = _load_expected(src.with_suffix("").with_suffix(".expected.toml"))
    blocks = find_acsl_blocks(src.read_bytes())
    detected_lines = [b.start_line for b in blocks]
    for forbidden in exp["must_not_be_detected"]:
        assert forbidden not in detected_lines, \
            f"line {forbidden} (string/char literal content) was incorrectly detected as ACSL"


# -----------------------------------------------------------
# Strip behavior: empty delete
# -----------------------------------------------------------

def test_empty_delete_is_identity(fixture, tmp_path):
    out = tmp_path / "out.c"
    strip(fixture["src"], out, [], fixture["expected"]["input_hash"])
    assert out.read_bytes() == fixture["content"]


# -----------------------------------------------------------
# Strip behavior: full delete removes all ACSL
# -----------------------------------------------------------

def test_full_delete_removes_all_acsl(fixture, tmp_path):
    out = tmp_path / "out.c"
    all_lines = fixture["expected"]["all_block_lines"]
    strip(fixture["src"], out, all_lines, fixture["expected"]["input_hash"])
    after = out.read_bytes()
    assert len(find_acsl_blocks(after)) == 0, \
        f"after full strip, ACSL blocks still remain"


# -----------------------------------------------------------
# Strip behavior: partial delete preserves count of others
# -----------------------------------------------------------

def test_partial_delete_preserves_count(fixture, tmp_path):
    """After deleting K of N blocks, exactly N-K blocks remain.

    Note: remaining blocks may shift to lower line numbers (multi-line
    deletions consume newlines). Only block COUNT is asserted here, not
    specific line numbers.
    """
    all_lines = fixture["expected"]["all_block_lines"]
    if len(all_lines) < 2:
        pytest.skip("fixture has < 2 blocks, partial test not meaningful")
    to_delete = [all_lines[0]]

    out = tmp_path / "out.c"
    strip(fixture["src"], out, to_delete, fixture["expected"]["input_hash"])
    remaining_blocks = find_acsl_blocks(out.read_bytes())
    assert len(remaining_blocks) == len(all_lines) - len(to_delete)


# -----------------------------------------------------------
# Strip behavior: C part byte-faithful
# -----------------------------------------------------------

def test_c_part_byte_faithful_after_full_strip(fixture, tmp_path):
    """After stripping ALL ACSL blocks, the non-ACSL byte ranges of the
    output should be byte-identical to the corresponding ranges in input."""
    all_lines = fixture["expected"]["all_block_lines"]
    out = tmp_path / "out.c"
    strip(fixture["src"], out, all_lines, fixture["expected"]["input_hash"])
    after = out.read_bytes()
    before = fixture["content"]

    # Build "C part" of input: bytes outside any ACSL block
    blocks = find_acsl_blocks(before)
    c_part_in = bytearray()
    prev = 0
    for b in blocks:
        c_part_in.extend(before[prev:b.start_byte])
        prev = b.end_byte
    c_part_in.extend(before[prev:])

    # After full strip, output should equal C part of input (all ACSL ranges gone)
    assert after == bytes(c_part_in)


# -----------------------------------------------------------
# Determinism: same input + same params → same output
# -----------------------------------------------------------

def test_determinism(fixture, tmp_path):
    all_lines = fixture["expected"]["all_block_lines"]
    out1 = tmp_path / "out1.c"
    out2 = tmp_path / "out2.c"
    strip(fixture["src"], out1, all_lines, fixture["expected"]["input_hash"])
    strip(fixture["src"], out2, all_lines, fixture["expected"]["input_hash"])
    assert out1.read_bytes() == out2.read_bytes()


# -----------------------------------------------------------
# Output is no larger than input (we only delete, never insert)
# -----------------------------------------------------------

def test_output_size_le_input(fixture, tmp_path):
    """Strip only removes bytes; output cannot be larger than input."""
    all_lines = fixture["expected"]["all_block_lines"]
    out = tmp_path / "out.c"
    strip(fixture["src"], out, all_lines, fixture["expected"]["input_hash"])
    assert len(out.read_bytes()) <= len(fixture["content"])


# -----------------------------------------------------------
# Hash mismatch → error, no output
# -----------------------------------------------------------

def test_hash_mismatch_raises(fixture, tmp_path):
    out = tmp_path / "out.c"
    bad_hash = "sha256:" + "0" * 64
    with pytest.raises(ValueError, match="hash mismatch"):
        strip(fixture["src"], out, [], bad_hash)
    assert not out.exists(), "output file must NOT be created on hash mismatch"


# -----------------------------------------------------------
# Missing block line → error, no output
# -----------------------------------------------------------

def test_missing_block_line_raises(fixture, tmp_path):
    out = tmp_path / "out.c"
    # Pick a line definitely not an ACSL block start
    bogus_line = 9999
    with pytest.raises(ValueError, match="not ACSL block starts"):
        strip(fixture["src"], out, [bogus_line], fixture["expected"]["input_hash"])
    assert not out.exists(), "output file must NOT be created on missing block error"


# -----------------------------------------------------------
# Hash format roundtrip
# -----------------------------------------------------------

def test_compute_hash_format():
    h = compute_hash(b"hello world")
    assert h.startswith("sha256:")
    assert len(h) == 7 + 64
    expected = hashlib.sha256(b"hello world").hexdigest()
    assert h == f"sha256:{expected}"
