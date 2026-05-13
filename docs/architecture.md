# FermatVerification-bench 架构设计

> 状态：架构阶段 v4 (2026-05-13) — A1 POC 完成后小修订
> v3 → v4：strip 工具后置条件修正（删严格幂等声明，改为 determinism + 输出单调）；§12.0 A1 标 ✅ 通过
> v2 → v3：吸收 v2 审查反馈，C1*(parse 段去主体性) / M5*(strip source_hash) / M2*(difficulty 由 classify 输出) 三项 Critical 落实，11 项 Major/Minor 一并修订
> 适用：本仓库 `FermatVerification-bench` 的总体结构、case manifest、工具体系、许可策略

---

## 1. 项目目标

提供一个**统一的 C + ACSL 验证基准库**，**verifier-agnostic**（不绑定任何特定验证器）。设计原则：

- **C + ACSL 是制品本身**。Bench 维护 (C, ACSL) 对、provenance、内在难度、特性标签等"用例自身的描述性数据"
- **跑验证是消费侧的事**。具体 verifier 的配置和结果**不进 bench 仓库**，由 consumer 外置管理
- **可重现**：上游依赖钉版本（submodule + commit pin / 数据集 hash），bench 自身用 SemVer
- **来源透明**：每个 case 标注上游来源 + 上游许可，**不混淆来源责任**

### 1.1 Bench 与 Consumer 的边界

| 是 bench 的责任 | 不是 bench 的责任 |
|---|---|
| 提供合法的 C 文件 + ACSL 注解块（**文本级**） | 这些注解在某 verifier 下是否可证 |
| 标注 provenance / license / difficulty / features | 跑某 verifier 的结果 |
| 提供 strip 工具产出双版本 | 双版本在某 verifier 下表现如何 |
| 上游 fetch / submodule 引用 | 上游内容能否在某 verifier 下 build |

---

## 2. 五部分总框架

每个 part 是按 **case 性质 + 测什么** 划分的逻辑分类。物理上每个 case 在唯一目录 `cases/<part_dir>/<source>/<id>/` 存在。

| Part | 名称 | 测什么 | 用例特征 |
|---|---|---|---|
| **1** | C 语言特性库 | C 语法构造的覆盖（差分 frontend 用） | 单文件、纯 C |
| **2** | ACSL 语言特性库 | ACSL 语法构造的覆盖 | 单文件、含 ACSL |
| **3** | 短小算法 (curated) | 函数级功能验证的典型案例 | 单/少文件、人工挑选 |
| **4** | 真实验证目标项目 | 多文件产品级 C+ACSL 项目 | 完整项目（多文件 + build system），带双版本 |
| **5** | mined 外部 corpus | 从公开数据集自动 ingest 的函数级 case | 函数级，按数据集分目录 |

注：列名是"测什么"（用例性质描述）而非"测试目标"（哪个工具能否过），保持 verifier-neutral 措辞。

---

## 3. 顶层目录结构

```
FermatVerification-bench/
├── README.md
├── LICENSE                          # 我们自己的贡献：MIT
├── NOTICE                           # 各上游来源 + 许可清单 + attribution
├── CHANGELOG.md                     # SemVer 版本变更记录
├── docs/
│   ├── architecture.md              # 本文档
│   ├── manifest-schema.md           # case.toml 字段权威说明（含词表、标度锚点）
│   ├── per-part/                    # 各 part 的细化文档（后续生成）
│   ├── variants.md                  # 双版本设计专项
│   ├── classify-prompts.md          # LLM classifier 的 prompt 设计
│   └── licenses.md                  # 许可策略详解
├── LICENSES/                        # 各上游许可原样保留
├── cases/                           # 详见 §3.2
├── tools/
│   ├── schema_version.py            # schema_version 单一来源（其他工具 import）
│   ├── validate_manifest.py
│   ├── build_views.py
│   ├── strip_acsl.py                # Layer 1: 机械删除
│   ├── classify_case.py             # Layer 2: LLM 判读 strip_policy + difficulty
│   ├── fetch_casp.py
│   ├── fetch_fm_bench.py
│   ├── tests/                       # pytest（后续工作）
│   └── lib/                         # 共享模块
├── views/                           # 由 build_views.py 自动生成
├── manifest.toml                    # 顶层引用文件 + bench 版本元信息
├── manifest_part1.toml ... manifest_part5.toml
└── .gitignore                       # 屏蔽 part5_mined/casp/*/{ground_truth,stripped}.c 等
```

### 3.1 命名规则统一

- **case id = 相对路径**（从 `cases/` 起）。例：`part3_curated/acsl_by_example/binary_search`
- **目录名 = id**。校验工具直接 `cases/{id}` 查找
- Part 目录名（固定）：`part1_c_features` / `part2_acsl_features` / `part3_curated` / `part4_real_targets` / `part5_mined`

### 3.2 `cases/` 子目录骨架

```
cases/
├── part1_c_features/
│   ├── goblintcil/
│   ├── frama_c_syntax/
│   ├── frama_c_cil/
│   └── inhouse/
├── part2_acsl_features/
│   ├── frama_c_wp_acsl/
│   ├── frama_c_spec/
│   └── inhouse/
├── part3_curated/
│   ├── frama_c_wp_gallery/
│   ├── acsl_by_example/
│   ├── inhouse_l1_pure/ ... inhouse_l6_callers/
│   └── frama_c_wp_models/           # wp_hoare/typed/bytes/region/tip 全部标 quality=wp_internal_regression
├── part4_real_targets/
│   └── anssi_x509_parser/           # 见 §6
│       ├── case.toml
│       ├── upstream/                # 嵌套 submodule
│       ├── transformations/
│       └── variants/
│           ├── ground_truth/
│           └── stripped/
└── part5_mined/
    ├── casp/
    │   └── _index.toml              # 元数据骨架，详见 docs/manifest-schema.md
    └── fm_bench_acsl/
```

---

## 4. Case manifest 规约

### 4.1 公共字段（所有 part 必填）

```toml
schema_version = "0.1"                # 来自 tools/schema_version.py 的单一来源
id            = "part3_curated/acsl_by_example/binary_search"
name          = "binary_search"
part          = 3

[provenance]
source        = "acsl_by_example"
upstream_url  = "https://github.com/fraunhoferfokus/acsl-by-example"
upstream_path = "StandardAlgorithms/BinarySearch/binary_search.c"
upstream_commit = "abc123..."                           # 可选
upstream_tool_hint = "frama-c 32 (Gallium)"             # 自由文本，可选；告知该用例最初针对哪个 verifier 写
license       = "MIT"                                   # SPDX 表达式（见 §9）
license_file  = "LICENSES/MIT-fraunhofer-fokus.txt"
quality       = "textbook"                              # 词表见下，作用域跨 part
intent        = "functional_correctness"                # 同上

[files]
entry_files   = ["binary_search.c"]                     # case 主要文件（多文件 part 4 case 列入口）
include_dirs  = []

[difficulty]
overall            = 2     # 0-5
proof_size         = 2     # 标度锚点详见 docs/manifest-schema.md
math_theory        = 1
data_complexity    = 2
control_complexity = 2
spec_complexity    = 2
auxiliary_needed   = ["loop_invariant"]   # 词表：loop_invariant | predicate_def | lemma | ghost | induction

[difficulty.scored_by]
type = "llm"                                # llm | human | human-override
model = "claude-opus-4-7"                   # type=llm 时必填
prompt_version = "v0.1"                     # type=llm 时必填
scored_at = "2026-05-12"

features = ["loops", "arrays_ro", "linear_arith"]
# 统一词表（跨 part）：
#   pure_arith | loops | nested_loops | arrays_ro | arrays_mut | pointers |
#   pointer_arithmetic | linked_struct | nested_struct | recursion | callers |
#   strings | bitwise | floats | overflow_concern |
#   ghost | axiomatic | logic_function | predicate | lemma | behaviors |
#   termination | linear_arith | nonlinear_arith
```

**词表说明（跨 part 通用）**：
- `quality`：`algorithm` | `textbook` | `acsl_feature_demo` | `wp_internal_regression` | `synthetic_corner` | `frontend_stress`
- `intent`：`functional_correctness` | `feature_demo` | `parser_stress` | `edge_case` | `proof_technique` | `memory_model_stress`

注：`shared_headers` 字段从 schema 删除（v3 修订）—— "每 case 拷一份"决策后该字段无信息含量。

### 4.2 Part 1 / Part 2 专用（前端覆盖类）

```toml
[parse]
constructs_used = ["gcc_extension", "designated_initializer"]
                                          # 用例里出现的语法/扩展构造，纯描述
                                          # 词表：gcc_extension | msvc_extension | acsl_extension |
                                          #       c99_only | c11_only | c17_only |
                                          #       vla | designated_initializer | bitfield |
                                          #       _Alignas | _Generic | statement_expression | etc.
known_issues    = ["lihaokun/FermatVerification#22"]   # 可选：链接已知 frontend gap issue
notes           = ""                      # 自由文本
```

注：**没有 `expected_outcome` / `divergence_kind` 字段**。Consumer 自己跑、自己判 pass/fail；bench 只描述用例性质。

### 4.3 Part 3 / 4 / 5 专用（功能验证类）

```toml
[verification]
target = ["functional"]                   # functional | total_correctness | memory_safety | termination
                                          # 词义详见 docs/manifest-schema.md

[variants]
ground_truth  = "variants/ground_truth/<file>"   # 可选；upstream 提供时填
stripped      = "variants/stripped/<file>"       # 必填

[variants.strip_policy]
goal_type     = "rte_only"                # rte_only | explicit_assert | explicit_lemma | mixed
goal_lines    = []                        # 若有显式 goal，列出"应保留"的 ACSL 块起始行
strip_lines   = [10, 25, 42, 87]          # "应删除"的 ACSL 块起始行
source_hash   = "sha256:abc123..."        # ground_truth 内容 hash；多文件用 "merkle:sha256:..."
rationale     = "纯函数级合约；verification target 是 RTE + 函数 ensures。"

[variants.strip_policy.classified_by]
type = "llm"                              # llm | human | upstream_split | human-override
model = "claude-opus-4-7"                 # type=llm 时必填
prompt_version = "v0.1"                   # type=llm 时必填
reviewed_by_human = false                 # part 4 必须 true 才能进入正式 case.toml
classified_at = "2026-05-12"
```

**人工 override 机制**：用户调整 LLM 自动结果后，将 `scored_by.type` 或 `classified_by.type` 改为 `"human-override"`，可选保留原值：

```toml
[difficulty]
overall = 4                               # 人工改的值
scored_by = { type = "human-override", scored_at = "2026-05-12" }
overridden_from = { overall = 3, scored_by = { type = "llm", model = "claude-opus-4-7" } }
```

### 4.4 Part 4 专用扩展（项目级）

```toml
[project]
upstream_repo     = "https://github.com/ANSSI-FR/x509-parser"
upstream_commit   = "..."
build_system      = "make"
loc               = 20122
function_count    = 87
upstream_verification_effort = "ANSSI 2018-2020"
verification_completeness    = "partial"  # full | partial | initial

[transformations]
sequence = ["01-flatten-includes.sh"]     # verifier-中立的预处理
notes    = ""
```

### 4.5 Part 5 专用扩展（mined 出处）

```toml
[mined]
source_dataset    = "casp"
upstream_id       = "frama_c_files_v2/test_frama_c_2054.c"   # CASP file_name
ultimate_origin   = "the_stack_v2"
ultimate_origin_path = ""                 # CASP 不映射回 The Stack；空值合法
per_file_license  = ""                    # 默认空；用户/工具如能查到原始可补
```

### 4.6 Schema 权威性

`docs/manifest-schema.md` 是字段定义、取值词表、**标度锚点**（如 `proof_size` 0-5 各档具体含义）的**唯一真实来源**。`tools/validate_manifest.py` 按它校验。本架构文档示例与 schema 文档冲突时，以 schema 为准。

---

## 5. 双版本设计专项

### 5.1 概念定义

| 概念 | 定义 |
|---|---|
| **Goal**（证明目标） | 整个证明体系的**终极**目标。**不**包括函数级摘要规约（function pre/post 是过程规约，不是 goal）。形态：① 顶层 `assert` / `main()` driver、② 显式 lemma 作为最终结论、③ 隐式的"WP + RTE 全过"（rte_only） |
| **Process spec**（过程规约） | 为了让 verifier 走通而引入的所有中间规约。**包括函数 pre/post**、loop invariants、ghost code、lemmas、predicate/logic function 定义、behaviors 等 |
| **Ground truth** | 上游作者提供的完整规约 = goal + process spec |
| **Stripped 版本** | 仅保留 goal（如有），删除所有 process spec |

**rte_only 类用例**（最常见）：没有显式 ACSL goal，stripped 输出是**纯 C** 文件，零 ACSL。`target = ["memory_safety"]` 隐式定义任务："让 WP+RTE 通过"。

### 5.2 Strip 工具两层架构

#### Layer 1: `tools/strip_acsl.py`（机械删除器）

**功能**：输入 ground_truth.c + 一组行号 + 期望的 source_hash，输出 stripped.c。**不判断**语义。

**接口**：
```
strip_acsl.py --input <file> --output <file> --delete-blocks <line-list> --expect-hash <sha256>
```

**实现策略**：文本扫描器（不走 AST）。

**前置条件**：
- 输入是合法文本（不要求合法 C+ACSL）
- `--delete-blocks` 给定的行号必须是某个 ACSL 块的**起始行**
- 输入的实际 hash 与 `--expect-hash` 匹配（不匹配立即报错，不输出 stripped）

**后置条件**：
- 输出的 **C 部分字节级一致**（仅删除被列出的 ACSL 块字节范围，C 代码字节不变）
- 输出的 ACSL 块仅保留**未被 `--delete-blocks` 列出**的
- **Determinism**：同输入 + 同 `--delete-blocks` + 同 `--expect-hash` → 输出字节完全相同
- **输出单调**：输出字节数 ≤ 输入字节数（strip 仅删除，不插入）

**注意（v3 → v4 修订）**：v3 曾声明"幂等：`strip(strip(x, L), L) == strip(x, L)`"。**该声明不成立**，因为多行 ACSL 块内含 `\n`，删除后 line numbers 下移，第二次 strip 时 L 列表里的行号不再对应 ACSL 块起始（触发"missing block"错误）。在当前设计（错误而非静默跳过 missing block）下，严格幂等不可能。**实际成立的属性是上面两条 Determinism + 单调。** Strip 是单向操作（ground_truth → stripped），不会被二次 strip，幂等不是需求。

**边界处理**：
- 字符串字面量内的 `/*@` 必须**不**被当作 ACSL 块
- ACSL 块内允许 `//` 行注释，扫描终止只看顶层 `*/`
- 行号给定但找不到对应 ACSL 块起始 → 报错，不静默跳过

#### Layer 2: `tools/classify_case.py`（LLM 判读 + difficulty 打分）

**功能**：读 case 内容 → 一次 LLM 调用 → 同时生成 `[variants.strip_policy]` 和 `[difficulty]` 写入 case.toml。

**接口**：
```
classify_case.py <case-id>
  → 写入 [variants.strip_policy] + [difficulty] + 计算 source_hash

classify_case.py <case-id> --reclassify
  → 重写两段（自动触发条件：source_hash 失效）

classify_case.py <case-id> --only strip
  → 仅刷新 strip_policy 段
classify_case.py <case-id> --only difficulty
  → 仅刷新 difficulty 段
```

**LLM 输出契约**：
```
goal_type     ∈ rte_only | explicit_assert | explicit_lemma | mixed
goal_lines    : List[int]
strip_lines   : List[int]
difficulty.*  : 6 个维度 0-5 整数 + auxiliary_needed list
rationale     : str
```

**Review 策略**：
- Part 3 / Part 5（大批量）：**信任 LLM**，结果直接写入 case.toml；`reviewed_by_human = false`
- Part 4（少量大用例）：**必须人工 review**。LLM 跑出建议 → 写到 `.classify-suggested.toml` → 人工修订后写入正式 case.toml；`reviewed_by_human = true`

**Source hash 失效处理**：`classify_case.py --reclassify` 自动覆盖时，**保留旧 hash + 旧 strip_policy 到 `.classify-archive/` 目录**，便于 diff 审查。

**特例**：CASP 的 case 跳过 LLM，因为上游已 split。`classified_by.type = "upstream_split"`。

### 5.3 已废弃：`entry_functions` 概念

v1 设计有此字段。v2 删除。LLM 直接按 ACSL 块逐块分类。

---

## 6. Part 4 项目级用例：以 ANSSI x509 为例

### 6.1 物理结构

```
cases/part4_real_targets/anssi_x509_parser/
├── case.toml
├── upstream/                    # 嵌套 submodule，pin 到 commit
├── transformations/             # verifier-中立的预处理（展平 include / 合并多文件等）
│   ├── 01-flatten-includes.sh
│   └── README.md
└── variants/
    ├── ground_truth/            # 应用 transformations 后 + 完整 704 ACSL
    │   └── ...
    └── stripped/                # ground_truth 经 strip_acsl.py 处理
        └── ...
```

**transformations/ 语义边界**：只做 verifier-中立预处理。**不**做 Frama-C 版本适配 —— 那是 consumer 的事。

### 6.2 case.toml 关键字段

```toml
schema_version = "0.1"
id   = "part4_real_targets/anssi_x509_parser"
name = "anssi_x509_parser"
part = 4

[provenance]
source = "anssi_x509_parser"
upstream_url = "https://github.com/ANSSI-FR/x509-parser"
upstream_commit = "<pin>"
upstream_tool_hint = "frama-c 18 (Argon)"
license = "BSD-2-Clause"                    # 从 dual GPLv2/BSD 选更宽松
license_file = "LICENSES/BSD-2-anssi.txt"
quality = "algorithm"
intent = "functional_correctness"

[project]
build_system = "make"
loc = 20122
upstream_verification_effort = "ANSSI 2018-2020"

[verification]
target = ["functional", "memory_safety"]    # 704 个 ACSL 里既有 RTE 也有功能正确性

[variants]
ground_truth = "variants/ground_truth"
stripped     = "variants/stripped"

[variants.strip_policy]
goal_type = "rte_only"                      # 即便有功能 contract，作为 stripped 任务时全删
goal_lines = []
strip_lines = [...]                         # 704 个 ACSL 块起始行
source_hash = "merkle:sha256:..."
rationale = "..."

[variants.strip_policy.classified_by]
type = "human-override"                     # part 4 必须人工 review
reviewed_by_human = true
classified_at = "2026-05-12"
```

### 6.3 构建流水

```
upstream/                  (submodule, immutable)
   ↓ apply transformations/01-*
variants/ground_truth/     (verifier-中立处理结果)
   ↓ tools/strip_acsl.py 按 strip_policy.strip_lines
variants/stripped/         (agent 评估的 task input)
```

variants/ 入版本库，避免每次跑 strip。

### 6.4 Part 5 (mined) 用例特例

**CASP**：
- 上游 parquet 天然 split（`verified_c_file_content` / `c_code_snippet` / `acsl_snippet`）
- `fetch_casp.py` 直接用：ground_truth = `verified_c_file_content`，stripped = `c_code_snippet`
- 不调 `classify_case.py`，约定 `goal_type = "rte_only"` / `classified_by.type = "upstream_split"`
- difficulty 字段需要单独跑 `classify_case.py --only difficulty`（CASP 本身不给）

**FM-Bench ACSL slice**：fetch 细节待 §12.0 POC 之后再定；可能需要跑 classify_case.py。

---

## 7. 工具体系

所有工具用 Python 3。不依赖任何 verifier 工具链。

### 7.1 `tools/schema_version.py`

```python
# 单一来源
SCHEMA_VERSION = "0.1"
```

其他工具 `from schema_version import SCHEMA_VERSION` 引用。Case.toml 写入时也用此值。

### 7.2 `tools/validate_manifest.py`

```
输入：cases/ 目录
输出：成功 → exit 0；失败 → 列违规
检查项：
  - schema_version 匹配（用 7.1 单一来源）
  - 必填字段存在 / 词表合法
  - id 字段与目录路径一致
  - license 字段是合法 SPDX 表达式（用 license-expression 库）
  - upstream_url 形态合法（不要求联网）
  - variants.strip_policy.source_hash 与 ground_truth 实际内容一致
  - variants.stripped 引用路径可选校验（--check-files 模式）
不检查：
  - .c 文件本身的 C/ACSL well-formedness
```

### 7.3 `tools/build_views.py`

```
输出：
  manifest.toml             # 顶层：bench 版本 + 5 个分片引用
  manifest_part1.toml ... manifest_part5.toml   # 各 part 分片
  views/by_part.md, by_source.md, by_difficulty.md, by_feature.md, stats.md
```

**分片 case 块缓存的关键字段**（v3 明确列出）：
```
id, path, source, license, quality, intent,
difficulty_overall, features, has_ground_truth, has_stripped
```

其他字段不缓存到分片；consumer 需要更多时读对应的 case.toml。

### 7.4 `tools/strip_acsl.py`

详见 §5.2 Layer 1。

### 7.5 `tools/classify_case.py`

详见 §5.2 Layer 2。

### 7.6 `tools/fetch_casp.py`

```
输入：无（HF dataset URL 内置）
输出：cases/part5_mined/casp/<id>/{ground_truth.c, stripped.c, case.toml}
副作用：
  - 创建 venv（若无）+ 装 pyarrow
  - 下载 train.parquet (~450 KB)
  - 直接用上游 split
  - 每行生成一个 case.toml（goal_type=rte_only, classified_by.type=upstream_split）
  - 不调 strip_acsl / classify_case
  - 可选：fetch 完后自动跑 classify_case.py --only difficulty（给每个 CASP case 打 difficulty）
依赖：python3, venv, 网络
不依赖：任何 verifier
```

### 7.7 `tools/fetch_fm_bench.py`

待 §12.0 POC 之后实现。FM-Bench ACSL 数据 jsonl 引用外部路径，实际 C 文件位置需调研。

### 7.8 共同要求

- 错误处理：失败不留半成品 → 写 `.in_progress` 标志，成功后删除
- 幂等：重复运行不破坏已生成产物（除非 `--force`）
- 单元测试：`tools/tests/` 用 pytest（细化阶段补）

---

## 8. 顶层 manifest 与分片

### 8.1 顶层 `manifest.toml`

```toml
schema_version = "0.1"
bench_version  = "0.1.0"            # SemVer
generated_at   = "2026-05-12T..."

[parts.part1_c_features]
manifest = "manifest_part1.toml"
[parts.part2_acsl_features]
manifest = "manifest_part2.toml"
# ...
```

### 8.2 分片 `manifest_partN.toml`

```toml
[[case]]
id = "part3_curated/acsl_by_example/binary_search"
path = "cases/part3_curated/acsl_by_example/binary_search"
source = "acsl_by_example"
license = "MIT"
quality = "textbook"
intent = "functional_correctness"
difficulty_overall = 2
features = ["loops", "arrays_ro", "linear_arith"]
has_ground_truth = true
has_stripped = true
```

字段集见 §7.3。

### 8.3 大小估计

总 case ~2000。每 case 在分片占 ~10 行。单分片最大 ~1MB（CASP 506 case），可接受。

---

## 9. 许可证策略

### 9.1 三层许可

1. **仓库层（我们的贡献）**：MIT。覆盖 `tools/`、`docs/`、`case.toml` 元数据、`transformations/` 中我们写的 patch
2. **来源层**：每个 source 在 `LICENSES/` 下放上游许可副本，`NOTICE` 汇总 attribution
3. **per-case 层**：每个 `case.toml` 的 `[provenance].license` 字段标该 case 许可

### 9.2 License 字段格式

**SPDX 表达式**：
- 单许可：`license = "MIT"`
- 双许可可选：`license = "GPL-2.0-only OR BSD-2-Clause"`
- 多许可都适用：`license = "MIT AND Apache-2.0"`
- 未确定：`license = "NOASSERTION"`（CASP 默认）
- 自定义：`license = "LicenseRef-MyCustom"`

校验工具用 Python `license-expression` 库；未知 SPDX ID 但语法合法允许通过。

### 9.3 CASP 不重分发

- 仓库**不**含 CASP 来源的 .c 文件
- `cases/part5_mined/casp/_index.toml` 记录 506 个 case 的元数据骨架（**详见 docs/manifest-schema.md**）
- 用户必须运行 `tools/fetch_casp.py` 拉取
- `.gitignore` 屏蔽 `cases/part5_mined/casp/**/ground_truth.c` 和 `**/stripped.c`
- 对 CASP，`[provenance].license = "NOASSERTION"`；`[mined].per_file_license = ""`

### 9.4 NOTICE 文件结构

```
NOTICE

This benchmark aggregates content from multiple upstream sources. Tool code
and metadata authored here are licensed under MIT (see LICENSE).

[fraunhofer_fokus_acsl_by_example]
URL: https://github.com/fraunhoferfokus/acsl-by-example
License: MIT
Affected: cases/part3_curated/acsl_by_example/**

[goblint_cil]
URL: https://github.com/goblint/cil
License: BSD-3-Clause
Affected: cases/part1_c_features/goblintcil/**

[frama_c_wp]
URL: https://git.frama-c.com/pub/frama-c
License: LGPL-2.1
Affected: cases/part2_acsl_features/frama_c_wp_acsl/**,
          cases/part3_curated/frama_c_wp_gallery/**,
          cases/part3_curated/frama_c_wp_models/**

[anssi_x509_parser]
URL: https://github.com/ANSSI-FR/x509-parser
License: dual GPL-2.0 / BSD-2-Clause; we redistribute under BSD-2-Clause
Affected: cases/part4_real_targets/anssi_x509_parser/upstream/** (git submodule)

[casp]
URL: https://huggingface.co/datasets/nicher92/CASP_dataset
License: per-file from The Stack v1+v2 (NOASSERTION at bench level)
Note: NOT redistributed. Run tools/fetch_casp.py to download.

[fm_bench_acsl]
URL: https://huggingface.co/datasets/fm-universe/FM-bench
License: Apache-2.0
Note: Only ACSL slice consumed; fetch via tools/fetch_fm_bench.py
```

---

## 10. 与外部仓库的接入

### 10.1 主仓库 FermatVerification 通过 submodule 引用

主仓库 commit `10f592d` 已注册：
```
[submodule "FermatVerification-bench"]
    path = FermatVerification-bench
    url = https://github.com/lihaokun/FermatVerification-bench.git
```

### 10.2 fv-core / frama-c-mcp 现有用例迁移

迁移规则（具体执行另开 PR）：
- `verification/L1-L6/*` → `cases/part3_curated/inhouse_l<N>_<name>/`
- `cil_features/` 中 C 特性子集 → `cases/part1_c_features/inhouse/`
- `cil_features/` 中 ACSL 特性子集 → `cases/part2_acsl_features/inhouse/`
- `cil_features/` 中真验证用例 → `cases/part3_curated/inhouse_<name>/`
- `x509/` → 删除（被 part4 取代）
- `regression/` → 归位 + manifest 标 `quality=synthetic_corner`
- `frama-c-mcp/test/x509-parser*/output*` → 移到主仓库 `output/` 或删除
- `frama-c-mcp/test/{array-utils,mini-sort,string-utils}/*.c` → 跟 inhouse 同处理

消费侧：fv-core 测试改用 submodule 路径 `FermatVerification-bench/cases/...`。

### 10.3 Verifier 结果数据外置

Bench case.toml 不含任何 verifier 跑出来的结果。Consumer 维护各自外置：
```
fv-core/bench-results/<case-id>/wp-typed-nocast.toml
agent-eval/results/<case-id>/<agent-name>-attempt-<n>.toml
```

外置文件格式由 consumer 自定。Bench 不约束。

### 10.4 依赖方向

```
fv-core / frama-c-mcp / agent  --(consume)-->  FermatVerification-bench
        (主仓库)                                  (本仓库，submodule)
```

Bench 仓库**不依赖**任何主仓库代码。

---

## 11. 关键设计决策与理由

| # | 决策 | 理由 |
|---|---|---|
| 1 | 独立仓库 + submodule 接入 | 多消费者、生命周期独立、可独立 cite/发布、许可隔离 |
| 2 | Bench 不维护 verifier 结果 | 保持 verifier-agnostic；多 consumer 不互相覆盖 |
| 3 | 5 部分按"测什么"分（性质而非结果） | 措辞中性，不预设 verifier；part 1/2 测前端、part 3/4/5 测功能 |
| 4 | wp_hoare/typed/bytes/region/tip 全收 + tag | 表面是 C+ACSL pair，纳入但 `quality=wp_internal_regression` 让 consumer 过滤 |
| 5 | GoblintCil tests 故意收 662 个，期望部分失败 | part 1 用差分测试暴露 frontend gap；失败本身是有用的输出 |
| 6 | CASP 不重分发，只发 fetch 脚本 | per-file 上游许可（The Stack 衍生）混进版本库 license 风险 |
| 7 | Part 4 双版本（ground_truth + stripped） | 评估时输入不能含答案；ground_truth 作为参考解 |
| 8 | Strip 工具两层（机械 Layer 1 + LLM Layer 2） | 工具不判语义；判读交给 LLM；策略与执行解耦 |
| 9 | 函数 pre/post 也算 process spec（**非直觉**） | "Goal"严格定义为"整个证明体系的终极目标"。函数级摘要规约只是"为了让 verifier 走通"的脚手架——即使是入口函数的 pre/post 也是脚手架（高层 goal 通常是顶层 assert / lemma / 或 RTE-freedom）。这跟"函数 pre/post 就是合约目标"的常见直觉不同，但符合 agent 评估范式 |
| 10 | License 字段 = SPDX 表达式 | 支持多许可、CASP NOASSERTION、自定义 LicenseRef- |
| 11 | Manifest 按 part 分片 | 单文件 2000 case 过大；分 5 片可控 |
| 12 | Bench 不做 well-formedness 检查 | 文本级聚合器，验证是 consumer 责任 |
| 13 | `[parse]` 段去主体性 | v1/v2 的 `expected_outcome` 隐含"某个特定 frontend"主体；v3 改为 `constructs_used` 纯描述 |
| 14 | Strip policy 带 source_hash | 防止 ground_truth 修改后 strip_lines 静默失效 |
| 15 | classify_case.py 一次出 strip + difficulty | LLM 已读完整 case，合并调用省资源 |

---

## 12. 风险与待决项

### 12.0 实现前必须验证的关键假设

进入物理填充前必须完成。任一失败需回到架构层重新讨论。

#### A1：Strip 工具文本扫描器边界情况

- 步骤：实现 strip_acsl.py POC → 在 wp_gallery 13 个用例上跑
- 验证标准：C 部分 byte-faithful + determinism + 输出单调 + source_hash 检测 + 不误删字符串字面量内的 `/*@`
- 失败处理：调整 strip 实现
- **状态**：✅ 已通过（commit 191bc73，46/46 tests pass）

#### A2：CASP fetch + parquet 解码

- 步骤：实现 fetch_casp.py POC → 生成 1-2 个 sample case
- 验证标准：case.toml 字段完整；上游 split 字段映射正确
- 失败处理：parquet 工具链调整 / schema 理解修正

#### A3：上游源可访问性 ✅

已通过：SURVEY.md 记录 5 个候选源全部 clone/download 成功。

### 12.1 已识别风险

1. **ANSSI x509 在 consumer-侧 verifier 上的可用性**：Bench 不关心；consumer 用 Frama-C 31 时可能要适配（与 Frama-C 18 时代差异）
2. **CASP 上游 file_name 不带 The Stack 原路径**：`[mined].ultimate_origin_path` 多数空。Per-file license 推断困难
3. **FM-Bench ACSL 数据位置**：jsonl 引用外部路径，实际 C 文件需调研后再定 fetch 实现

### 12.2 后续工作（不在架构范围）

- Part 1-5 细化设计文档（`docs/per-part/*.md`）
- Strip 工具细化设计（`docs/strip-tool-design.md`）
- LLM classifier prompt 设计（`docs/classify-prompts.md`）
- `docs/manifest-schema.md` 完整撰写（含标度锚点、词表权威定义、_index.toml schema）
- 现有 fv-core / frama-c-mcp 用例迁移 PR
- CI 设计（GitHub Actions）
- 首版填充计划
- `CONTRIBUTING.md`
- `tools/tests/` pytest 框架
- `tools/migrate_schema.py`（schema 版本迁移工具）
- 正式 `README.md`（覆盖 gh 自动生成的占位）

---

## 13. 评审清单

提交 v3 评审前请逐项确认：

- [ ] 5 部分定位与各 part 的 source 列表是否符合预期？
- [ ] manifest schema 字段是否充分？是否仍有遗漏？
- [ ] §4.2 `[parse]` 段（去 expected_outcome）是否清晰？
- [ ] §4.3 strip_policy.source_hash + classified_by 结构化是否清晰？
- [ ] §5 双版本 + strip 两层架构是否清晰？
- [ ] §5.2 classify_case.py 同时出 strip + difficulty 是否符合预期？
- [ ] Bench / consumer 边界划分（§1.1 + §10.3）是否清晰？
- [ ] CASP 不重分发 + per-file license 处理是否可接受？
- [ ] License SPDX 表达式策略 + 三层结构是否清晰？
- [ ] 与主仓库 FermatVerification 的接入方式是否合理？
- [ ] 关键设计决策表（§11，15 项）是否完整无遗漏？
- [ ] §12.0 的 POC 关卡（A1+A2）是否覆盖关键风险？
- [ ] 是否同意进入细化设计阶段？

---

## 附录：版本变更摘要

### v3 → v4

| 类别 | 主要变化 |
|---|---|
| Strip 工具后置条件 | §5.2 Layer 1：删除"幂等：strip(strip(x, L), L) == strip(x, L)"声明；改为 Determinism（同输入+同参数→同输出）+ 输出单调（输出字节 ≤ 输入字节）。理由：多行 ACSL 块删除后 line numbers 下移，再 strip 同一行号会触发 missing block error。Strip 单向 ground_truth → stripped，不会二次 strip，幂等不是真实需求 |
| §12.0 A1 状态 | 标记 ✅ 已通过（commit 191bc73, 46/46 tests pass） |

### v2 → v3

| 类别 | 主要变化 |
|---|---|
| Critical | C1\* `[parse]` 去主体性（删 expected_outcome / divergence_kind，改 constructs_used）/ M5\* strip_policy 加 source_hash + strip_acsl 验证 / M2\* classify_case.py 合并输出 strip + difficulty |
| Major | M1\* difficulty 标度锚点放 manifest-schema.md / M3\* 删 shared_headers / M4\* x509 target 改 functional+memory_safety / M6\* classified_by 结构化（type/model/prompt_version/reviewed_by_human）|
| Minor | m1\* §2 表 verifier-neutral / m2\* _index.toml 指向 manifest-schema.md / m3\* schema_version.py 单一来源 / m4\* §11 #9 说明加粗"非直觉"提示 / m5\* README 列入 §12.2 / m6\* 词表跨 part 注明 / m7\* §7.3 分片关键字段列出 |
| 决策表扩充 | §11 决策表从 12 项扩到 15 项（加 #13/#14/#15）|

### v1 → v2

去 Frama-C 中心化；Strip 两层架构；Goal 定义修正；废弃 `entry_functions`；schema 字段 id/license/verification 大改；manifest 分片；详见 v2 版附录。
