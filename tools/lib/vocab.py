"""Authoritative vocabulary constants for case.toml validation.

Source of truth: docs/manifest-schema.md §15.
"""

from __future__ import annotations

# §15.1 source
SOURCE = frozenset({
    "goblintcil",
    "frama_c_syntax",
    "frama_c_cil",
    "frama_c_wp_acsl",
    "frama_c_spec",
    "frama_c_wp_gallery",
    "frama_c_wp_models",
    "acsl_by_example",
    "anssi_x509_parser",
    "casp",
    "fm_bench_acsl",
    "svcomp",
    "inhouse",
    "inhouse_l1_pure",
    "inhouse_l2_loops",
    "inhouse_l3_readonly_arrays",
    "inhouse_l4_mutable_arrays",
    "inhouse_l5_pointers",
    "inhouse_l6_callers",
})

# §15.2 quality
QUALITY = frozenset({
    "algorithm",
    "textbook",
    "acsl_feature_demo",
    "wp_internal_regression",
    "synthetic_corner",
    "frontend_stress",
})

# §15.3 intent
INTENT = frozenset({
    "functional_correctness",
    "feature_demo",
    "parser_stress",
    "edge_case",
    "proof_technique",
    "memory_model_stress",
})

# §15.4 features (24 项)
FEATURES = frozenset({
    # data (8)
    "pure_arith", "arrays_ro", "arrays_mut", "pointers",
    "pointer_arithmetic", "linked_struct", "nested_struct", "strings",
    # control (4)
    "loops", "nested_loops", "recursion", "callers",
    # numeric (5)
    "bitwise", "floats", "overflow_concern", "linear_arith", "nonlinear_arith",
    # acsl (7)
    "ghost", "axiomatic", "logic_function", "predicate", "lemma",
    "behaviors", "termination",
})

# §15.5 auxiliary_needed
AUXILIARY = frozenset({
    "loop_invariant",
    "predicate_def",
    "lemma",
    "ghost",
    "induction",
})

# §15.6 classified_by.type / scored_by.type
CLASSIFIED_BY_TYPE = frozenset({
    "llm",
    "human",
    "upstream_split",
    "human-override",
})

# §15.7 constructs_used
CONSTRUCTS = frozenset({
    "gcc_extension", "msvc_extension", "acsl_extension",
    "c99_only", "c11_only", "c17_only",
    "vla", "designated_initializer", "bitfield",
    "_Alignas", "_Generic", "statement_expression",
})

# §15.8 verification.target
TARGET = frozenset({
    "functional",
    "total_correctness",
    "memory_safety",
    "termination",
})

# §15.9 goal_type
GOAL_TYPE = frozenset({
    "rte_only",
    "explicit_assert",
    "explicit_lemma",
    "mixed",
})

# §15.10 build_system
BUILD_SYSTEM = frozenset({
    "make",
    "cmake",
    "dune",
    "none",
})

# §15.11 mined.source_dataset
SOURCE_DATASET = frozenset({
    "casp",
    "fm_bench_acsl",
    "svcomp",
})

# verification_completeness (§11)
VERIFICATION_COMPLETENESS = frozenset({
    "full",
    "partial",
    "initial",
})

# Part directory prefixes
PART_DIR_PREFIX = {
    1: "part1_c_features",
    2: "part2_acsl_features",
    3: "part3_curated",
    4: "part4_real_targets",
    5: "part5_mined",
}
