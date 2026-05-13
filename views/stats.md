# Bench Statistics

> Generated: 2026-05-13T20:39:34Z
> Bench version: 0.1.0 (schema: 0.1)

**Total cases**: 1345

## By Part

| Part | Name | Count |
|---|---|---|
| 1 | part1_c_features | 662 |
| 2 | part2_acsl_features | 88 |
| 3 | part3_curated | 89 |
| 4 | part4_real_targets | 0 |
| 5 | part5_mined | 506 |

## By Source

| Value | Count |
|---|---|
| `goblintcil` | 662 |
| `casp` | 506 |
| `frama_c_wp_acsl` | 88 |
| `acsl_by_example` | 76 |
| `frama_c_wp_gallery` | 13 |

## By Quality

| Value | Count |
|---|---|
| `frontend_stress` | 662 |
| `algorithm` | 519 |
| `acsl_feature_demo` | 88 |
| `textbook` | 76 |

## By Intent

| Value | Count |
|---|---|
| `parser_stress` | 662 |
| `functional_correctness` | 588 |
| `feature_demo` | 88 |
| `edge_case` | 5 |
| `proof_technique` | 2 |

## By License

| Value | Count |
|---|---|
| `BSD-3-Clause` | 662 |
| `NOASSERTION` | 506 |
| `LGPL-2.1-only` | 101 |
| `MIT` | 76 |

## Difficulty Distribution (overall)

| Level | Count |
|---|---|
| 0 | 506 |
| 1 | 662 |
| 2 | 109 |
| 3 | 47 |
| 4 | 21 |
| 5 | 0 |

## Feature Tags (20 distinct)

| Feature | Count |
|---|---|
| `loops` | 84 |
| `predicate` | 67 |
| `arrays_ro` | 56 |
| `pointers` | 50 |
| `nested_struct` | 48 |
| `arrays_mut` | 36 |
| `linear_arith` | 24 |
| `axiomatic` | 15 |
| `behaviors` | 14 |
| `floats` | 13 |
| `termination` | 13 |
| `logic_function` | 12 |
| `strings` | 7 |
| `nested_loops` | 7 |
| `bitwise` | 6 |
| `pure_arith` | 4 |
| `lemma` | 3 |
| `overflow_concern` | 1 |
| `callers` | 1 |
| `ghost` | 1 |

## Variants Coverage

- Cases with `ground_truth`: 595 / 1345
- Cases with `stripped`:     595 / 1345
