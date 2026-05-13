"""Single source of truth for the bench schema + bench version.

See docs/manifest-schema.md §1 for upgrade policy.
"""

# Schema version (manifest format). Bump per §19 SemVer.
SCHEMA_VERSION = "0.1"

# Bench dataset version (collection of cases). Bump per CHANGELOG.md.
# - Major: 不兼容的 case 删除/移动
# - Minor: 新增/丰富 case
# - Patch: bug 修复（修 case.toml 字段错等）
BENCH_VERSION = "0.1.0"
