# Transformations Applied to ANSSI x509-parser Upstream

Generated: 2026-05-14

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
