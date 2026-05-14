# Changelog

All notable changes to this project are documented here.
Bench versioning follows SemVer:
- **Major**: incompatible case/structure changes
- **Minor**: significant new sources or case additions
- **Patch**: bug fixes / small metadata corrections

## [Unreleased] / v0.2.0 dev

### 添加（pending v0.2.0 release）

- ✅ **ANSSI x509-parser part 4 case** —— submodule + concat MVP
  - `cases/part4_real_targets/anssi_x509_parser/upstream/` git submodule
    pinned to commit `6f3bae3` (ANSSI-FR/x509-parser)
  - `tools/ingest_anssi_x509.py`: concat 全部 .h + .c → `variants/ground_truth/x509_flat.c`（595 KB / 20164 行）
  - strip → `variants/stripped/x509_flat.c`（455 KB）
  - 683 ACSL 块识别 + 删除
  - 单文件 concat 适配 v0.1 schema（strip_lines 单 list），未引入 multi-file 扩展
  - `LICENSES/BSD-2-anssi.txt` attribution + dual GPLv2/BSD 选 BSD-2-Clause

### 待添加

- (C) `tools/classify_case.py` —— LLM 自动判读 strip_policy + difficulty
- 现有 1933 case 的 difficulty / features LLM 重打分（取代 v0.1 的 human/heuristic）

---

## [0.1.0] — 2026-05-14 (initial release)

### 添加 — 架构与工具

- Architecture v4 final（5 部分 + verifier-agnostic + 双版本设计）
- Manifest schema v0.1（case.toml + 5 manifest 分片 + views）
- `tools/schema_version.py` — SCHEMA_VERSION + BENCH_VERSION 单一来源
- `tools/lib/vocab.py` — 11 个权威词表
- `tools/strip_acsl.py` — Layer 1 机械 ACSL 删除器 + 46 单元测试
- `tools/validate_manifest.py` — case.toml schema 校验（字段级 / 字段间 / 文件存在 / source_hash 一致）
- `tools/build_views.py` — manifest.toml + 5 分片 + 5 markdown views 聚合
- `tools/fetch_casp.py` — CASP HuggingFace 数据下载 + ingest
- `tools/ingest_wp_gallery.py` — Frama-C wp_gallery 13 case 入库
- `tools/ingest_acsl_by_example.py` — ACSL by Example 76 case（#include 展平）
- `tools/ingest_wp_acsl.py` — Frama-C wp_acsl 88 case 入库
- `tools/ingest_goblintcil.py` — GoblintCil 662 case 入库
- `tools/ingest_frama_c_tests.py` — Frama-C tests/{syntax,cil,spec} 587 case 入库

### 添加 — 用例（**1932 case**，所有 5 part 中 4 个已填）

| Part | Cases | Sources |
|---|---|---|
| 1 — C 语言特性 | 1022 | GoblintCil (662) + Frama-C tests/syntax (334) + tests/cil (26) |
| 2 — ACSL 语言特性 | 315 | Frama-C wp_acsl (88) + Frama-C tests/spec (227) |
| 3 — curated functional | 89 | Frama-C wp_gallery (13) + ACSL by Example (76) |
| 4 — real targets | 0 | (ANSSI x509 待 v0.2 ingest) |
| 5 — mined corpus | 506 | CASP |

### 添加 — 文档

- `README.md` — 项目入口 + 快速开始 + 工具表 + 索引
- `docs/architecture.md` v4 — 总体架构与设计决策
- `docs/manifest-schema.md` v2 — 字段权威定义 + 词表 + 难度标度锚点
- `LICENSE` — MIT（适用本仓库的工具/元数据/文档）
- `NOTICE` — per-source attribution（6 个上游源）
- `LICENSES/` — 各上游许可副本（MIT-fraunhofer-fokus / LGPL-2.1-frama-c /
  BSD-3-goblint / CASP-attribution）

### 决策与排除

- **CASP 不重分发**：仓库只发 `tools/fetch_casp.py`；`.c` 文件 `.gitignore`，
  首次使用前必须跑 fetch 脚本
- **ANSSI x509 part 4 推迟**：multi-file 项目级 ingest 需要 schema 设计
  （submodule + transformations + per-file strip），留 v0.2 处理
- **FM-Bench ACSL 排除**：调研后确认 FM-Bench ACSL 切片（119 examples）是
  ACSL by Example 算法的 LLM 任务重排（SegGen / ReqAna / ProofGen / Code2Proof /
  ProofInfill），跟我们已有的 76 个 ACSL by Example case 内容重叠 + 任务形态
  不是独立 C+ACSL pair。不适合作为独立 verification bench case
- **FormalBench 排除**：HF dataset 100% Java，README 说"支持 C/ACSL"但
  没实际数据（详见 `bench-research/SURVEY.md`）

### 已知限制

- 1932 case 的 `[difficulty]` 字段是 v0.1 启发式 / 手工初始值，未经 LLM
  精细评分；`classify_case.py`（v0.2）将批量重打
- `[parse].constructs_used` 也是文件名启发式；未经实际 parser 验证
- POC 阶段：consumer-侧 verifier 结果数据外置，无 CI 集成验证
- Part 4 为空（ANSSI x509 待 v0.2）

---

## 编号约定

| 类别 | 标签 | 说明 |
|---|---|---|
| **Add** | `feat:` | 新功能 / 新用例 / 新工具 |
| **Fix** | `fix:` | bug 修复 |
| **Docs** | `docs:` | 文档变更 |
| **Refactor** | `refactor:` | 代码重组，无功能变化 |
| **Chore** | `chore:` | 工具链 / submodule / CI |
