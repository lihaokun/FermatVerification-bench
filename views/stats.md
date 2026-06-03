# Bench Statistics

> Generated: 2026-06-03T09:42:17Z
> Bench version: 0.1.0 (schema: 0.1)

**Total cases**: 2878

## By Part

| Part | Name | Count |
|---|---|---|
| 1 | part1_c_features | 1042 |
| 2 | part2_acsl_features | 324 |
| 3 | part3_curated | 207 |
| 4 | part4_real_targets | 1 |
| 5 | part5_mined | 1304 |

## By Source

| Value | Count |
|---|---|
| `goblintcil` | 662 |
| `casp` | 506 |
| `live_fm_bench` | 360 |
| `frama_c_syntax` | 334 |
| `fm_bench_verified` | 280 |
| `frama_c_spec` | 227 |
| `svcomp` | 158 |
| `frama_c_wp_acsl` | 88 |
| `frama_c_wp_models` | 81 |
| `acsl_by_example` | 76 |
| `inhouse` | 37 |
| `frama_c_cil` | 26 |
| `frama_c_wp_gallery` | 13 |
| `inhouse_l1_pure` | 6 |
| `inhouse_l2_loops` | 6 |
| `inhouse_l3_readonly_arrays` | 6 |
| `inhouse_l4_mutable_arrays` | 5 |
| `inhouse_l5_pointers` | 4 |
| `inhouse_l6_callers` | 2 |
| `anssi_x509_parser` | 1 |

## By Quality

| Value | Count |
|---|---|
| `algorithm` | 1355 |
| `frontend_stress` | 1040 |
| `acsl_feature_demo` | 324 |
| `wp_internal_regression` | 81 |
| `textbook` | 76 |
| `synthetic_corner` | 2 |

## By Intent

| Value | Count |
|---|---|
| `functional_correctness` | 1424 |
| `parser_stress` | 1040 |
| `feature_demo` | 324 |
| `memory_model_stress` | 63 |
| `proof_technique` | 20 |
| `edge_case` | 7 |

## By License

| Value | Count |
|---|---|
| `Apache-2.0` | 798 |
| `LGPL-2.1-only` | 769 |
| `BSD-3-Clause` | 662 |
| `NOASSERTION` | 506 |
| `MIT` | 142 |
| `BSD-2-Clause` | 1 |

## Difficulty Distribution (overall)

| Level | Count |
|---|---|
| 0 | 1146 |
| 1 | 1054 |
| 2 | 461 |
| 3 | 190 |
| 4 | 26 |
| 5 | 1 |

## Feature Tags (21 distinct)

| Feature | Count |
|---|---|
| `loops` | 279 |
| `linear_arith` | 200 |
| `arrays_mut` | 141 |
| `arrays_ro` | 92 |
| `predicate` | 72 |
| `nested_struct` | 70 |
| `pointers` | 70 |
| `behaviors` | 26 |
| `axiomatic` | 25 |
| `logic_function` | 25 |
| `floats` | 19 |
| `termination` | 18 |
| `strings` | 15 |
| `pure_arith` | 15 |
| `ghost` | 8 |
| `nested_loops` | 8 |
| `bitwise` | 6 |
| `lemma` | 4 |
| `callers` | 3 |
| `recursion` | 1 |
| `overflow_concern` | 1 |

## Variants Coverage

- Cases with `ground_truth`: 1152 / 2878
- Cases with `stripped`:     1512 / 2878
