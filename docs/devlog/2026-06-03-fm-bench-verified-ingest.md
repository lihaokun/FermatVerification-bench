# FM-Bench-Verified (280 case) 入库 Part 5

日期：2026-06-03
分支：`add-casp-and-livefm`

## 做了什么

把 HuggingFace 数据集 `fm-universe/FM-bench-verified`（280 个经 Frama-C WP 验证的 C+ACSL case）作为 `fm_bench_verified` source 正式入库 Part 5。

- 启用 source `fm_bench_verified`（此前是预留/EXCLUDED；本次会话先把旧名 `fm_bench_acsl` 重命名为 `fm_bench_verified`，含子表名 `[mined.fm_bench_verified]`）。
- `validate_manifest.py::check_mined` 给 `fm_bench_verified` 分支补齐子表字段检查（task/folder/file_name/dataset_source）。
- Schema 文档 §13 示例 + §13.3 字段表定稿（去掉占位措辞）。
- 新工具 `tools/ingest_fm_bench_verified.py`（纯 stdlib，322 行）：拉取 `fm-bench-verified.jsonl` → 缓存 → 生成 280 case。
- 新许可文件 `LICENSES/Apache-2.0-fm-bench-verified.txt` + NOTICE 条目（EXCLUDED → INGESTED）。
- 重生成 manifest + views；README / tools/README / CHANGELOG 同步。

## 关键决策

- **CASP 式双版本建模**：与 Live-FM-Bench（task-only）相反，本数据集上游 `output` 全部非空（280 个全过 WP 的 ground-truth C+ACSL）。所以 `ground_truth.c = output`、`stripped.c = input`，`source_hash` 绑定 ground_truth —— `--check-strip-hash` 会**真正校验** hash 一致性（比 task-only 更强）。
- **goal_type 逐 case 计算**：从 ground_truth 判定，explicit_assert(254) / mixed(26)。
- **目录按 source 列分组**：`fm_bench_verified/{autospec,githubRepository,fmbench}/<name>/`，便于溯源（数据集本身就按这三源统计）。
- **保留全部 280**，不与 part3 `acsl_by_example`(76) 去重（其中 ~45 个经 FM-bench 源自 ACSL-by-Example）。这是独立清洗 + WP 验证的双版本重构，与 part3 单文件制品不同。
- **复用 Live-FM-Bench 范式**：format_toml_string / sanitize_segment / 主循环结构直接沿用；纯 stdlib jsonl（无 pyarrow），本机可跑可验证。

## 验证

- `ingest_fm_bench_verified.py` → `emitted 280 cases`，280 ground_truth.c + 280 stripped.c。
- `build_views.py` → 2878 case；`--check` up-to-date。
- `validate_manifest.py --check-files --check-strip-hash` → fm_bench_verified **0 error**，**含 source_hash 一致性**（全 280 双版本 hash 对得上）。全仓 1518 error 仍全部来自 CASP 的 gitignored `.c`（环境态）。
- `pytest tools/tests/` → **46 passed**（无回归）。
- 抽查 3 个跨 source case（autospec/github/fmbench）：ground_truth==output、stripped==input、source_hash 对、dataset_source 对、goal_type 合理。

## 注意 / 遗留

- `fm_bench_acsl → fm_bench_verified` 重命名时踩到子串坑：`fm_bench` 是 `live_fm_bench`(1115×) 和 `fm_bench_verified` 的子串，**禁止全局 sed**，必须定点改。
- **已清理**：原始 FM-bench（`fm-universe/FM-bench`, arXiv:2501.16207）此前已 EXCLUDED（CHANGELOG 有记录），其预留工具 `fetch_fm_bench.py` 既无 source 槽位也无 ingest 计划，故移除全部引用（README / tools/README / architecture.md 工具树 + §7.7 + provenance 块）。§7.7 改写为两个真实 ingest 工具（live_fm_bench + fm_bench_verified）的说明；architecture.md provenance 块补 `[live_fm_bench]` + 修正 `[fm_bench_verified]` URL/Note。

## 度量

| 指标 | 数值 |
|------|------|
| 新增代码行数 | ~340（ingest 工具 322 + validator ~8 + schema/doc ~15） |
| 修改代码行数 | ~10（含 fm_bench_acsl→verified 重命名定点改 ~10 处） |
| 删除代码行数 | ~15（NOTICE EXCLUDED 段重写） |
| 涉及文件数 | 工具/文档/词表/许可 ~8 个 + 生成 280 case × 3 文件 + manifest/views 11 个 |
| 新增用例数 | 280 |
| 新增测试用例数 | 0（沿用 validate/build_views/strip 测试链）|
| 测试通过率 | validate fm_bench_verified 280/280 无 error（含 hash）；pytest 46/46 |
| 发现 bug 数 | 0 |
| 修复 bug 数 | 0 |
| 迭代轮次 | 调研 1 / 实现 1 |
