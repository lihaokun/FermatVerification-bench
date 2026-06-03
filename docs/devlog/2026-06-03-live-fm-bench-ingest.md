# Live-FM-Bench (360 case) 入库 Part 5

日期：2026-06-03
分支：`add-casp-and-livefm`

## 做了什么

把 HuggingFace 数据集 `fm-universe/Live-FM-Bench`（360 个 sv-comp25 Code2Proof case）作为新 source `live_fm_bench` 接入 Part 5。

- 新增 source `live_fm_bench`（`tools/lib/vocab.py` 的 `SOURCE` + `SOURCE_DATASET`）。
- 新增校验分支：`validate_manifest.py::check_mined` 要求 `source_dataset=live_fm_bench` 时存在 `[mined.live_fm_bench]` 子段（`task`/`folder`/`file_name`/`dataset_source`）。
- Schema 文档 `docs/manifest-schema.md`：§13.4 新子段、§14 互斥表、§15.1/§15.11 词表（顺带补回此前缺失的 `svcomp` source 文档行），并在 §10 写明 **task-only** case 的 ground_truth 可省略约定。
- 新工具 `tools/ingest_live_fm_bench.py`（纯 stdlib，无 pyarrow）：拉取上游 `live-fm-bench.jsonl` → 缓存 `tools/.cache/live_fm_bench/` → 逐行生成 360 个 case。
- 新许可文件 `LICENSES/Apache-2.0-live-fm-bench.txt` + `NOTICE` 条目。
- 重生成 manifest + views；README / tools/README 同步。

## 关键决策

- **task-only 建模**：Live-FM-Bench 是 contamination-free 的 held-out 评测集，上游 `output`（gold ACSL 解）全空。所以每个 case 只产 `stripped.c`（= 上游 `input`，含 `//@ assert` 待验证目标），**不产 `ground_truth.c`**。schema 本就支持（stripped 必填、ground_truth 可选），`source_hash` 改绑 `stripped.c` 内容，`§16.4` 一致性检查在无 ground_truth 时自动跳过 —— 无需改该检查。这是本仓库第一个 task-only 来源。
- **直接 commit `.c`**：数据集 apache-2.0 且仅 ~262KB，故源码直接入库（区别于 CASP 的 fetch + gitignore 模式）。
- **不与现有 `svcomp`(158) 去重**：底层程序虽重叠（loop-lit/array-examples），但框架不同（ACSL Code2Proof vs 原始 `__VERIFIER_assert` + YAML），两者并存。
- **用 jsonl 而非 parquet**：上游原始文件是 `live-fm-bench.jsonl`，stdlib `json` 即可解析，工具无 pyarrow 依赖、在无 venv 的机器上也能跑。`filefolder_name` 上游全局唯一，目录无碰撞。

## 验证

- `python3 tools/ingest_live_fm_bench.py` → `emitted 360 cases`，360 个 `stripped.c`、0 个 `ground_truth.c`。
- `build_views.py` → 2598 case；`--check` 报 up-to-date。
- `validate_manifest.py --check-files --check-strip-hash` → live_fm_bench **0 error**。（全仓 1518 error 全部来自 CASP 的 gitignored `.c`，需先跑 `fetch_casp.py`，属环境态，与本次无关。）
- 抽查 3 个跨类别 case：`stripped.c == 上游 input`、无 ground_truth、TOML 可解析、goal_lines 正确。
- `py_compile` 三个改动的 Python 文件通过。

## 踩到 / 注意

- 本机无 `pip` / `python3-venv` / `pytest`：`pytest tools/tests/` 无法运行，但未触碰 `strip_acsl.py`，无回归风险。→ 已回写 CLAUDE.md「已知限制」。
- `validate_manifest.py` 从路径反推 case_id（`relative_to(cases/)`），故无法对 `/tmp` 下的临时 case 校验，只能在真实 `cases/` 位置验证。
- `build_views.py` 每次重写全部 5 个分片，part1-4 仅 `generated_at` 时间戳变化（0 内容变更）—— 提交时这是预期 churn。

## 度量

| 指标 | 数值 |
|------|------|
| 新增代码行数 | ~360（ingest 工具 312 + vocab/validator ~10 + schema/doc ~70） |
| 修改代码行数 | ~10（vocab 2 + validator 8） |
| 删除代码行数 | 0 |
| 涉及文件数 | 工具/文档/词表/许可 8 个 + 生成 360 case × 2 文件 + manifest/views 11 个 |
| 新增用例数 | 360 |
| 新增测试用例数 | 0（沿用现有 validate/build_views 校验链）|
| 测试通过率 | validate live_fm_bench 360/360 无 error；pytest 环境不可用 |
| 发现 bug 数 | 0 |
| 修复 bug 数 | 0（顺带补 schema 文档缺失的 svcomp 词表行）|
| 迭代轮次 | 调研 1 / 实现 1 |
