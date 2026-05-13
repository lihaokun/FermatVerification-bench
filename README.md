# FermatVerification-bench

> 统一的 **C + ACSL 验证基准库**，verifier-agnostic 设计，分 5 部分覆盖前端、规约、算法、真实项目、mined corpus。

[![Schema](https://img.shields.io/badge/schema-0.1-blue)](docs/manifest-schema.md)
[![Bench](https://img.shields.io/badge/bench-0.1.0-green)](views/stats.md)
[![License](https://img.shields.io/badge/tools-MIT-orange)](LICENSE)

---

## 这是什么

提供 **C + ACSL 制品**作为验证基准 —— 而不是绑定某个 verifier。具体 verifier（Frama-C/WP、CBMC、Verifast、未来的 LLM agent prover...）跑出的结果由 consumer 外置管理，**不进 bench 仓库**。

设计原则与边界划分详见 [`docs/architecture.md`](docs/architecture.md)。

---

## 5 部分框架

| Part | 名称 | 测什么 | 用例特征 |
|---|---|---|---|
| **1** | C 语言特性 | C 语法构造的覆盖（差分 frontend 用） | 单文件、纯 C |
| **2** | ACSL 语言特性 | ACSL 语法构造的覆盖 | 单文件、含 ACSL |
| **3** | 短小算法 (curated) | 函数级功能验证典型案例 | 单/少文件、人工挑选 |
| **4** | 真实验证目标项目 | 多文件产品级 C+ACSL 项目 | 完整项目（多文件 + build system），带双版本 |
| **5** | mined 外部 corpus | 从公开数据集自动 ingest 的函数级 case | 函数级，按数据集分目录 |

每个 case 都带：
- **provenance**：来源 + 许可 + 上游路径
- **难度评分**（6 维 + 综合 + auxiliary needed）
- **特性标签**（features）
- **双版本**（part 3/4/5）：`ground_truth.c` + `stripped.c` + `strip_policy`

---

## 当前状态

详见 [`views/stats.md`](views/stats.md)。

- **Bench 版本**：0.1.0
- **Schema 版本**：0.1
- **总用例数**：519
  - Part 5 (mined corpus)：506 CASP
  - Part 3 (curated)：13 Frama-C wp_gallery
  - Part 1 / 2 / 4：填充中

详见 [`views/stats.md`](views/stats.md) / [`views/by_part.md`](views/by_part.md)。

---

## 快速开始

### 作为 submodule 接入

```bash
git submodule add https://github.com/lihaokun/FermatVerification-bench.git bench
git submodule update --init --recursive
```

### 独立 clone

```bash
git clone https://github.com/lihaokun/FermatVerification-bench.git
cd FermatVerification-bench
```

### Fetch CASP 数据（首次必做）

CASP 不重分发 —— per-file 上游许可需用户自查。`ground_truth.c` 和 `stripped.c` 默认 .gitignore，要跑以下脚本才能拿到：

```bash
python3 -m venv .venv
.venv/bin/pip install pyarrow
.venv/bin/python tools/fetch_casp.py
# → 下载 ~445KB parquet → 写出 506 个 cases/part5_mined/casp/<id>/{ground_truth.c, stripped.c}
```

### 验证 manifest 正确性

```bash
python3 tools/validate_manifest.py --check-files --check-strip-hash
```

应输出 `0 error(s)`。

### 重新生成 manifest + views

修改 `cases/` 后：

```bash
python3 tools/build_views.py             # 写 manifest.toml + 5 分片 + 5 views
python3 tools/build_views.py --check     # CI 模式：仅校验是否最新
```

---

## 工具

详见 [`tools/README.md`](tools/README.md)。

| 工具 | 用途 |
|---|---|
| `tools/strip_acsl.py` | Layer 1: 机械删除指定行的 ACSL 块（文本扫描器，C 部分 byte-faithful） |
| `tools/classify_case.py` | Layer 2: LLM 判读 `[variants.strip_policy]` + `[difficulty]`（**待实现**） |
| `tools/validate_manifest.py` | case.toml schema 校验（字段级 / 字段间 / 文件存在 / source_hash 一致） |
| `tools/build_views.py` | 聚合 case.toml → manifest.toml + 5 分片 + 5 markdown views |
| `tools/fetch_casp.py` | 从 HuggingFace 下载 CASP parquet 并生成 part5_mined/casp 用例 |
| `tools/fetch_fm_bench.py` | FM-Bench ACSL slice fetch（**待实现**） |
| `tools/schema_version.py` | `SCHEMA_VERSION` / `BENCH_VERSION` 常量单一来源 |
| `tools/lib/vocab.py` | 11 个权威词表（source / quality / intent / features / ...） |

---

## 文档

| 文档 | 内容 |
|---|---|
| [`docs/architecture.md`](docs/architecture.md) | 总体架构、5 部分定位、设计决策、风险与 TODO |
| [`docs/manifest-schema.md`](docs/manifest-schema.md) | case.toml 字段权威定义、词表、0–5 难度标度锚点、校验规则 |
| `docs/per-part/` | 各 part 的细化设计（**待补**） |
| `docs/classify-prompts.md` | LLM classifier prompt 设计（**待补**） |

---

## 索引文件

- [`manifest.toml`](manifest.toml) — 顶层 bench 元数据 + 5 个分片引用
- `manifest_part{1..5}.toml` — 按 part 分片的 case 索引
- [`views/stats.md`](views/stats.md) — 综合统计（按 part / source / quality / difficulty 等）
- [`views/by_part.md`](views/by_part.md) — 按 part 列出全部 case
- [`views/by_source.md`](views/by_source.md) — 按来源分组
- [`views/by_difficulty.md`](views/by_difficulty.md) — 按难度排序
- [`views/by_feature.md`](views/by_feature.md) — 按特性标签分组

---

## License

- **工具代码 + 元数据 + 文档**：MIT（详见 [`LICENSE`](LICENSE)）
- **`cases/**/` 内的源码**：保留各自上游许可（详见 [`NOTICE`](NOTICE) + 每个 `case.toml` 的 `[provenance].license` 字段 + [`LICENSES/`](LICENSES/) 目录）

特别注意：
- **CASP** 来源 case 是 The Stack v1+v2 衍生，每文件上游许可需用户自查
- **ANSSI x509** 是 dual GPL-2.0 / BSD-2-Clause，我们以 BSD-2-Clause 重分发

---

## 引用

如果你在论文或工作中使用本 bench，请：

1. 引用本仓库：`https://github.com/lihaokun/FermatVerification-bench`
2. 单独引用使用到的上游来源，比如：
   - **CASP**：arXiv:2508.18798
   - **ACSL by Example**：https://github.com/fraunhoferfokus/acsl-by-example
   - **FM-Bench**：arXiv:2501.16207
   - 其他详见 [`NOTICE`](NOTICE)

---

## 项目状态

POC 阶段（v0.1）：
- ✅ 架构 v4 + Schema v0.1 定稿
- ✅ Strip 工具（Layer 1）+ 46 单元测试
- ✅ validate_manifest + build_views
- ✅ CASP 全 506 case 入库（含 case.toml 元数据；.c 文件 fetch 时生成）
- ✅ Frama-C wp_gallery 13 case 入库（part 3）
- ⏳ classify_case.py（LLM 判读 strip_policy + difficulty）
- ⏳ ACSL by Example / GoblintCil / Frama-C tests/ 入库
- ⏳ ANSSI x509 (Part 4) submodule + transformations
- ⏳ FM-Bench ACSL slice fetch

详见 [`docs/architecture.md` §12](docs/architecture.md)。

---

## Contributing

`CONTRIBUTING.md` 待补。新增 source 或新 case 的流程详见 [`docs/manifest-schema.md`](docs/manifest-schema.md) §15 词表扩展约定。

---

## 父项目

本仓库是 **[FermatVerification](https://github.com/lihaokun/FermatVerification)**（LLM-agent 驱动的 C 程序自动形式化验证工具集）的 bench submodule。
