#!/usr/bin/env python3
"""Strip ACSL annotation blocks from a C / C+ACSL source file.

Layer 1 mechanical deleter. See docs/architecture.md §5.2 and
docs/manifest-schema.md §10.2 for design.

Usage:
    strip_acsl.py --input <file> --output <file> \\
                  --delete-blocks <comma-separated-line-list> \\
                  --expect-hash <sha256-hex>

Semantics:
    - Find all ACSL annotation blocks in input: /*@ ... */ and //@ ... <eol>
    - Delete those whose START LINE (1-indexed) is in --delete-blocks
    - Replace deleted block's BYTE RANGE with nothing (preserve surrounding
      whitespace and newlines so line numbers do not shift)
    - C / non-ACSL content is byte-faithful

Validation:
    - Verifies sha256(input) matches --expect-hash before any output
    - Each line in --delete-blocks must correspond to an existing ACSL block start
      (otherwise: error, no output produced)

Edge cases handled:
    - String literals ("..." with \\ escapes) — /*@ inside string is NOT an ACSL block
    - Char literals ('...' with \\ escapes) — same
    - Regular C block comments /* ... */ (no @) — recognized but not treated as ACSL
    - Regular C line comments // ... — same
    - ACSL block comment containing // inside — // is part of ACSL content, not a comment terminator
"""

from __future__ import annotations

import argparse
import hashlib
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Sequence


@dataclass(frozen=True)
class AcslBlock:
    """An ACSL annotation block detected in source.

    Attributes:
        start_line: 1-indexed line where the block opens (`/*@` or `//@`)
        start_byte: byte offset of the opening character
        end_byte: byte offset PAST the terminator
                  - for /*@...*/ blocks: byte after the closing `/`
                  - for //@... line blocks: byte at the terminating `\\n` (newline NOT included)
        is_line_block: True for //@ style, False for /*@ */ style
    """
    start_line: int
    start_byte: int
    end_byte: int
    is_line_block: bool


def find_acsl_blocks(content: bytes) -> list[AcslBlock]:
    """Scan content byte-by-byte; return all ACSL blocks in source order.

    State machine over input bytes; tracks line number through `\\n` occurrences,
    including inside strings/chars/comments. Never enters ACSL detection inside
    string / char literals.
    """
    blocks: list[AcslBlock] = []
    i = 0
    line = 1
    n = len(content)

    while i < n:
        c = content[i]

        # newline (in NORMAL state)
        if c == 0x0A:  # b'\n'
            line += 1
            i += 1
            continue

        # string literal
        if c == 0x22:  # b'"'
            i += 1
            while i < n:
                ci = content[i]
                if ci == 0x5C and i + 1 < n:  # b'\\'
                    if content[i + 1] == 0x0A:
                        line += 1
                    i += 2
                    continue
                if ci == 0x22:
                    i += 1
                    break
                if ci == 0x0A:
                    line += 1
                i += 1
            continue

        # char literal
        if c == 0x27:  # b"'"
            i += 1
            while i < n:
                ci = content[i]
                if ci == 0x5C and i + 1 < n:  # b'\\'
                    if content[i + 1] == 0x0A:
                        line += 1
                    i += 2
                    continue
                if ci == 0x27:
                    i += 1
                    break
                if ci == 0x0A:
                    line += 1
                i += 1
            continue

        # `/` — possible comment opening
        if c == 0x2F and i + 1 < n:
            c1 = content[i + 1]

            # /* or /*@ — block comment / ACSL
            if c1 == 0x2A:  # b'*'
                is_acsl = (i + 2 < n and content[i + 2] == 0x40)  # b'@'
                start_line = line
                start_byte = i
                i += 2  # skip /*
                # find closing */
                end_byte: int | None = None
                while i + 1 < n:
                    ci = content[i]
                    if ci == 0x0A:
                        line += 1
                    if content[i] == 0x2A and content[i + 1] == 0x2F:
                        i += 2
                        end_byte = i
                        break
                    i += 1
                if end_byte is None:
                    # unterminated block comment — eat rest of file
                    end_byte = n
                    i = n
                if is_acsl:
                    blocks.append(AcslBlock(
                        start_line=start_line,
                        start_byte=start_byte,
                        end_byte=end_byte,
                        is_line_block=False,
                    ))
                continue

            # // or //@ — line comment / ACSL line
            if c1 == 0x2F:  # b'/'
                is_acsl = (i + 2 < n and content[i + 2] == 0x40)  # b'@'
                start_line = line
                start_byte = i
                i += 2  # skip //
                # find newline (NOT consumed)
                while i < n and content[i] != 0x0A:
                    i += 1
                end_byte = i
                if is_acsl:
                    blocks.append(AcslBlock(
                        start_line=start_line,
                        start_byte=start_byte,
                        end_byte=end_byte,
                        is_line_block=True,
                    ))
                continue

        i += 1

    return blocks


def compute_hash(content: bytes) -> str:
    """Compute 'sha256:<hex>' digest of given bytes."""
    return "sha256:" + hashlib.sha256(content).hexdigest()


def strip(
    input_path: Path,
    output_path: Path,
    delete_blocks: Sequence[int],
    expect_hash: str,
) -> None:
    """Strip ACSL blocks whose start_line is in delete_blocks.

    Validates input hash before any write. Each line in delete_blocks must
    correspond to an existing ACSL block; otherwise raises ValueError without
    producing output.
    """
    content = input_path.read_bytes()

    actual_hash = compute_hash(content)
    if expect_hash != actual_hash:
        raise ValueError(
            f"source hash mismatch:\n"
            f"  expected: {expect_hash}\n"
            f"  actual:   {actual_hash}\n"
            f"The input file has changed since classification. "
            f"Run classify_case.py --reclassify <case-id> to regenerate strip_policy."
        )

    blocks = find_acsl_blocks(content)
    by_line = {b.start_line: b for b in blocks}

    delete_set = set(delete_blocks)
    missing = sorted(delete_set - set(by_line.keys()))
    if missing:
        raise ValueError(
            f"--delete-blocks references lines {missing} which are not ACSL block starts.\n"
            f"ACSL blocks found at lines: {sorted(by_line.keys())}"
        )

    to_delete = sorted(
        [by_line[ln] for ln in delete_set],
        key=lambda b: b.start_byte,
    )

    out = bytearray()
    prev_end = 0
    for blk in to_delete:
        out.extend(content[prev_end:blk.start_byte])
        prev_end = blk.end_byte
    out.extend(content[prev_end:])

    output_path.write_bytes(bytes(out))


def main() -> None:
    p = argparse.ArgumentParser(
        description="Strip ACSL annotation blocks at specified lines (Layer 1 mechanical).",
    )
    p.add_argument("--input", required=True, type=Path,
                   help="Input C / C+ACSL file (ground_truth)")
    p.add_argument("--output", required=True, type=Path,
                   help="Output stripped file")
    p.add_argument("--delete-blocks", required=True,
                   help="Comma-separated list of ACSL block start line numbers (1-indexed). "
                        "Empty list ('') means no deletion.")
    p.add_argument("--expect-hash", required=True,
                   help="Expected sha256 hash of input file, formatted as 'sha256:<64-hex>'")
    args = p.parse_args()

    s = args.delete_blocks.strip()
    delete_blocks = [int(x.strip()) for x in s.split(",") if x.strip()] if s else []

    try:
        strip(args.input, args.output, delete_blocks, args.expect_hash)
    except ValueError as e:
        print(f"strip_acsl: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
