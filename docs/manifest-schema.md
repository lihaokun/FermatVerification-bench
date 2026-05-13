# Manifest Schema 权威定义

> 状态：v2 (2026-05-13)
> v1 → v2：修 §6 编号混乱（features 与 [difficulty] 分离为独立 §6 / §7）；4 项决策落实（difficulty 用途 disclaimer / quality vs intent 区分 / part 4 review 软化 / CASP 元数据进 case.toml）；补齐 features 24 项定义 + constructs_used 语法例
> 适用：本仓库所有 `case.toml`、`manifest.toml`、`manifest_partN.toml`、特殊 `_index.toml` 文件的字段定义、取值词表、标度锚点、校验规则
> 关系：架构文档（`architecture.md`）示例与本文档冲突时，以本文档为准

---

## 0. 阅读指引

| 你要找什么 | 看哪一节 |
|---|---|
| case.toml 整体长什么样 | §2 |
| 某个字段是必填还是可选 / 取值范围 | §3–§13 按段落查 |
| 难度评分 0-5 各档具体含义 | §7.3 |
| 词表全集（quality / intent / features / ...）| §15 |
| `validate_manifest.py` 检查哪些规则 | §16 |
| 顶层和分片 manifest 长什么样 | §17 |
| CASP 等特殊 `_index.toml` | §18 |
| Schema 升级怎么做 | §19 |

---

## 1. Schema 版本

- 当前版本：**`schema_version = "0.1"`**
- 权威常量来源：`tools/schema_version.py`
- 每个 `case.toml` 必填 `schema_version` 字段，与上述常量必须严格相等
- 升级策略详见 §19

---

## 2. case.toml 总体结构

```toml
schema_version = "0.1"
id            = "..."
name          = "..."
part          = 1..5

[provenance]                          # §4，所有 part 必填
[files]                               # §5，所有 part 必填
features = [...]                      # §6，所有 part 必填
[difficulty]                          # §7，所有 part 必填
[difficulty.scored_by]                # §7.5

[parse]                               # §8，仅 part 1/2
[verification]                        # §9，仅 part 3/4/5
[variants]                            # §10，仅 part 3/4/5
[variants.strip_policy]               # §10.2
[variants.strip_policy.classified_by] # §10.3

[project]                             # §11，仅 part 4
[transformations]                     # §12，仅 part 4

[mined]                               # §13，仅 part 5
```

段间约束（哪些段必须配合）见 §14。

---

## 3. 公共字段 — 标识

| 字段 | 类型 | 必填 | 说明 |
|---|---|---|---|
| `schema_version` | str | ✓ | 必须等于 `tools/schema_version.py` 中常量 |
| `id` | str | ✓ | 相对 `cases/` 的目录路径。例：`part3_curated/acsl_by_example/binary_search`。**必须**等于 case.toml 所在目录的相对路径 |
| `name` | str | ✓ | 人可读名称。无格式约束，但建议与 id 最后一段相同或意义等价 |
| `part` | int | ✓ | 1, 2, 3, 4, 或 5。**必须**与 id 前缀（`part1_c_features/...` 对应 1，等等）一致 |

---

## 4. `[provenance]` 段

```toml
[provenance]
source             = "<词表见 §15.1>"
upstream_url       = "https://..."
upstream_path      = "..."
upstream_commit    = "..."                    # 可选
upstream_tool_hint = "..."                    # 可选
license            = "<SPDX 表达式>"
license_file       = "LICENSES/<filename>"
quality            = "<词表见 §15.2>"
intent             = "<词表见 §15.3>"
```

### 4.1 字段细节

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `source` | str | ✓ | 必须在词表中（§15.1）|
| `upstream_url` | str | ✓ | http/https URL；不要求联网验证 |
| `upstream_path` | str | ✓ | 相对路径，在 upstream 仓库中的位置；CASP 等 mined case 可填上游 `file_name` |
| `upstream_commit` | str |   | 上游 commit SHA；强烈建议 part 4 必填 |
| `upstream_tool_hint` | str |   | 自由文本。例："frama-c 18 (Argon)"、"CBMC 5.95"、"未知" |
| `license` | str | ✓ | 合法 SPDX 表达式（详见 §4.2）|
| `license_file` | str | ✓ | 相对仓库根的路径，指向 `LICENSES/` 下文件 |
| `quality` | str | ✓ | 必须在词表中（§15.2）|
| `intent` | str | ✓ | 必须在词表中（§15.3）|

### 4.2 License 字段格式（SPDX 表达式）

合法形态：

| 形态 | 例 |
|---|---|
| 单许可（SPDX ID） | `"MIT"` |
| 双许可可选 | `"GPL-2.0-only OR BSD-2-Clause"` |
| 多许可都适用 | `"MIT AND Apache-2.0"` |
| 自定义许可 | `"LicenseRef-MyCustom"` |
| 未确定 | `"NOASSERTION"` |
| WITH 表达式 | `"GPL-2.0-only WITH Classpath-exception-2.0"` |

校验：用 Python `license-expression` 库 parse。未知 SPDX ID 但语法合法允许通过（仅 warning，不报错）。

特例：
- **CASP 用例**：`license = "NOASSERTION"`（每 file 上游许可需用户自查）
- **多许可可选**：consumer 选择哪个由其自身策略决定；bench 不强制

### 4.3 `quality` 与 `intent` 的区分

两个字段都描述用例属性但维度不同，**不冗余**：

| 字段 | 维度 | 回答 |
|---|---|---|
| `quality` | **来源/性质** | 这个用例来自哪类活动？ |
| `intent` | **测试用途** | 拿这个用例来测什么最合适？ |

大多数 case 两个字段值有典型搭配，但**允许跨搭配**：

| 典型搭配 | 例 |
|---|---|
| `quality=algorithm` + `intent=functional_correctness` | wp_gallery 的 bsearch |
| `quality=textbook` + `intent=functional_correctness` | ACSL by Example 的 binary_search |
| `quality=acsl_feature_demo` + `intent=feature_demo` | wp_acsl 的 ghost 演示 |
| `quality=wp_internal_regression` + `intent=memory_model_stress` | wp_typed 用例 |
| `quality=frontend_stress` + `intent=parser_stress` | GoblintCil 测试 |

**跨搭配**（说明二者非 1-to-1）：
| 跨搭配 | 例 |
|---|---|
| `quality=algorithm` + `intent=edge_case` | 真实算法用例放在边界条件测试集 |
| `quality=textbook` + `intent=proof_technique` | 教材 case 用来演示 induction 技巧 |
| `quality=algorithm` + `intent=memory_model_stress` | 真实算法但用于压测特定内存模型 |

→ 因此两个字段都保留，**作用域跨 part**。

---

## 5. `[files]` 段

```toml
[files]
entry_files  = ["<file>.c", ...]
include_dirs = ["<rel-path>", ...]            # 可选，默认 []
```

### 5.1 字段细节

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `entry_files` | list[str] | ✓ | case 目录内的主要源文件。Part 3 单文件 case 通常一个；part 4 多文件项目列入口集 |
| `include_dirs` | list[str] |   | 相对 case 目录的 include 路径；consumer 编译时用 |

注：v2 schema 中没有 `shared_headers` 字段（曾在 v1 存在；"每 case 拷一份"决策后该字段无信息含量，已删除）。

---

## 6. `features` 字段

```toml
features = ["loops", "arrays_ro", "linear_arith"]
```

| 类型 | 必填 | 约束 |
|---|---|---|
| list[str] | ✓ | 允许空 list；元素必须在 §15.4 词表中；跨 part 通用 |

`features` 描述 case 的**语义/算法特性**（一段 C+ACSL "测什么内容"）。与 part 1/2 的 `[parse].constructs_used`（语法层面）区分：

| 维度 | features | constructs_used |
|---|---|---|
| 层面 | 语义 / 算法 | 语法 / 词法 |
| 例 | `loops` / `pointers` / `recursion` | `gcc_extension` / `vla` / `_Generic` |
| 作用域 | 所有 part | 仅 part 1/2 |

24 个词的完整定义见 §15.4。

---

## 7. `[difficulty]` 段

```toml
[difficulty]
overall            = 0..5
proof_size         = 0..5
math_theory        = 0..5
data_complexity    = 0..5
control_complexity = 0..5
spec_complexity    = 0..5
auxiliary_needed   = ["<词表见 §15.5>"]
```

### 7.1 难度评分的用途与精度说明

**这些 0–5 评分仅作为"粗略 sort / filter / 概览"指标**，不是精确量化。

✅ 适合的用途：
- 按难度分桶筛选 case（"给我 overall ≥ 3 的所有 case"）
- 排序展示（views 按难度排）
- 给人快速感知用例规模
- LLM agent 看到难度提前调整策略

❌ **不适合**的用途：
- 直接用 `proof_size` 数字回归预测 WP 实际 discharge 时间
- 用 `math_theory` 等级精确比较"哪个理论难"
- 在论文里用难度评分跨 case 做精确难度比较

LLM 自动评分的内在主观性：同一 case 不同 LLM / 不同 prompt 版本可能差 1 档（甚至 2）。**容忍这种噪声**；如果需要精确指标，consumer 应基于实际 verifier 结果（外置数据）自己量化。

### 7.2 字段细节

| 字段 | 类型 | 必填 | 取值 |
|---|---|---|---|
| `overall` | int | ✓ | 0–5；综合难度（非自动聚合，由 LLM/人独立给）|
| `proof_size` | int | ✓ | 0–5；锚点见 §7.3 |
| `math_theory` | int | ✓ | 0–5；锚点见 §7.3 |
| `data_complexity` | int | ✓ | 0–5；锚点见 §7.3 |
| `control_complexity` | int | ✓ | 0–5；锚点见 §7.3 |
| `spec_complexity` | int | ✓ | 0–5；锚点见 §7.3 |
| `auxiliary_needed` | list[str] | ✓ | 允许空 list；元素见 §15.5 |

### 7.3 0–5 标度锚点（**权威定义**）

#### overall（综合难度）

| 档 | 含义 | 典型例 |
|---|---|---|
| 0 | trivial：返回常量 / 直接传参，无逻辑 | `int id(int x){return x;}` |
| 1 | simple：纯算术，无循环无指针 | `int abs(int x)` |
| 2 | easy：单层有界循环，scalar/array，基础不变式 | `factorial`、`sum_n` |
| 3 | moderate：嵌套循环 或 指针操作，非平凡不变式 | `binary_search`、`reverse` |
| 4 | hard：归纳 over 数据结构 / 需自定义 lemma / ghost | `linked_list_reverse` |
| 5 | very hard：自定义理论 / 复杂手工证明 / 复合堆 | x509 内核函数、复杂位运算证明 |

#### proof_size（所需 ACSL 标注规模）

| 档 | ACSL 块数 |
|---|---|
| 0 | 0–2 |
| 1 | 3–10 |
| 2 | 11–30 |
| 3 | 31–100 |
| 4 | 101–300 |
| 5 | > 300 |

#### math_theory（所需数学理论）

| 档 | 理论 |
|---|---|
| 0 | 纯布尔 / 常量比较 |
| 1 | Presburger（线性整数算术 + 同余）|
| 2 | 线性算术 + 数组理论 |
| 3 | 非线性算术 / bitvector |
| 4 | 混合理论 / 归纳 over inductive type |
| 5 | 自定义理论 / 用户定义 logic function |

#### data_complexity（数据结构复杂度）

| 档 | 数据 |
|---|---|
| 0 | 无状态（纯函数）|
| 1 | 仅 scalar |
| 2 | 数组（一维）|
| 3 | 单层指针 / 二维数组 / 简单 struct |
| 4 | linked list / tree / 嵌套 struct |
| 5 | 复合堆 / 共享别名 / 复杂图结构 |

#### control_complexity（控制流复杂度）

| 档 | 控制流 |
|---|---|
| 0 | 直线（无分支）|
| 1 | 简单 if / switch |
| 2 | 单层有界循环 |
| 3 | 嵌套循环 / 多层分支 |
| 4 | 递归 |
| 5 | 互递归 / 复杂 CFG / irreducible loops |

#### spec_complexity（规约复杂度）

| 档 | ACSL 形态 |
|---|---|
| 0 | 无 ACSL spec（仅 RTE）|
| 1 | 仅 pre/post conditions |
| 2 | 含 behaviors / disjoint/complete |
| 3 | 含 ghost / axiomatic |
| 4 | 含 lemma / 辅助 predicate 定义 |
| 5 | 含 induction + 自定义 lemma / 复杂证明结构 |

### 7.4 `auxiliary_needed` 字段

```toml
auxiliary_needed = ["loop_invariant", "predicate_def"]
```

值见 §15.5 词表。表示验证此 case 通常**需要 agent 额外引入**的辅助构件类型。空 list 表示不需要额外辅助。

### 7.5 `[difficulty.scored_by]` 段

```toml
[difficulty.scored_by]
type           = "llm" | "human" | "human-override" | "upstream_split"
model          = "..."                        # type=llm 时必填
prompt_version = "..."                        # type=llm 时必填
scored_at      = "YYYY-MM-DD"                 # ISO 日期
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `type` | str | ✓ | 词表见 §15.6 |
| `model` | str | type=llm 必填 | LLM model identifier。例：`"claude-opus-4-7"` |
| `prompt_version` | str | type=llm 必填 | 对应 `docs/classify-prompts.md` 中的版本号 |
| `scored_at` | str | ✓ | ISO 8601 日期（不含时分）|

### 7.6 人工 override 机制

用户手工调整 LLM 自动评分后：

```toml
[difficulty]
overall = 4                                   # 改后的值
proof_size = 2
# ... 其他字段也可能被覆盖

[difficulty.scored_by]
type = "human-override"
scored_at = "2026-05-13"

[difficulty.overridden_from]                  # 可选；保留 LLM 原值留底
overall = 3
proof_size = 2
# ...
[difficulty.overridden_from.scored_by]
type = "llm"
model = "claude-opus-4-7"
prompt_version = "v0.1"
scored_at = "2026-05-12"
```

`overridden_from` 段非必填，但**建议**保留以便审计。

---

## 8. `[parse]` 段（仅 part 1 / part 2）

```toml
[parse]
constructs_used = ["<词表见 §15.7>"]
known_issues    = ["lihaokun/FermatVerification#22"]   # 可选
notes           = ""                                    # 可选
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `constructs_used` | list[str] | ✓ | 允许空；元素见 §15.7 |
| `known_issues` | list[str] |   | 已知问题链接（`owner/repo#N` 格式）|
| `notes` | str |   | 自由文本 |

注：v1 曾有 `expected_outcome` / `divergence_kind` 字段，v3 删除（详见 architecture v3 决策 #13）。`constructs_used` 是纯描述性词表，**不**带"该 case 应该 parse 成功/失败"语义。

---

## 9. `[verification]` 段（仅 part 3 / 4 / 5）

```toml
[verification]
target = ["functional"]                       # 词表见 §15.8
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `target` | list[str] | ✓ | 至少一个；元素见 §15.8 |

### 9.1 Target 词义

| 值 | 定义 |
|---|---|
| `functional` | partial correctness：前条件下，若终止则后条件成立 |
| `total_correctness` | `functional` + termination：前条件下必定终止且后条件成立 |
| `memory_safety` | 无内存安全错误：no OOB / no UAF / no null deref / no uninit read。等价于 Frama-C `-wp-rte` 默认子集（含整数溢出检查）|
| `termination` | 函数必定终止（ACSL `decreases` / `terminates \true`）|

允许组合，如 `["functional", "memory_safety"]`、`["memory_safety", "termination"]`。

---

## 10. `[variants]` 段（仅 part 3 / 4 / 5）

```toml
[variants]
ground_truth = "variants/ground_truth/<file>"   # 可选；upstream 提供时填
stripped     = "variants/stripped/<file>"       # 必填
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `ground_truth` | str |   | 相对 case 目录的路径；上游提供 ground truth 时填 |
| `stripped` | str | ✓ | 相对 case 目录的路径；必须实际存在 |

### 10.1 多文件 case（part 4）

Part 4 项目类 case 的 `ground_truth` 和 `stripped` 可以指向**目录**而非单文件：

```toml
[variants]
ground_truth = "variants/ground_truth"          # 目录
stripped     = "variants/stripped"              # 目录
```

校验时检查目录下文件结构对应。

### 10.2 `[variants.strip_policy]` 段

```toml
[variants.strip_policy]
goal_type     = "<词表见 §15.9>"
goal_lines    = [int, ...]                    # 可空（rte_only 时通常为空）
strip_lines   = [int, ...]                    # 必填
source_hash   = "sha256:<hex>"                # 或 "merkle:sha256:<hex>"
rationale     = "..."
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `goal_type` | str | ✓ | 词表见 §15.9 |
| `goal_lines` | list[int] | ✓ | "应保留"的 ACSL 块起始行号；rte_only 时通常为 [] |
| `strip_lines` | list[int] | ✓ | "应删除"的 ACSL 块起始行号 |
| `source_hash` | str | ✓ | 见 §10.4 |
| `rationale` | str | ✓ | LLM 或人工给出的判断理由 |

### 10.3 `[variants.strip_policy.classified_by]` 段

```toml
[variants.strip_policy.classified_by]
type              = "llm" | "human" | "upstream_split" | "human-override"
model             = "..."                     # type=llm 时必填
prompt_version    = "..."                     # type=llm 时必填
reviewed_by_human = false                     # part 4 强烈建议 true（不强制）
classified_at     = "YYYY-MM-DD"
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `type` | str | ✓ | 词表见 §15.6 |
| `model` | str | type=llm 必填 | LLM model identifier |
| `prompt_version` | str | type=llm 必填 | 对应 `docs/classify-prompts.md` 版本号 |
| `reviewed_by_human` | bool | ✓ | part 4 case **强烈建议** true（v2 软化为 warning，不再 hard fail）|
| `classified_at` | str | ✓ | ISO 日期 |

**关于 Part 4 review 软化**（v1 → v2）：v1 要求 Part 4 case 必须 `reviewed_by_human = true`，否则 validate 报错。v2 改为：若 Part 4 case 的 `reviewed_by_human = false`，validate 输出 **warning**（"建议人工 review part 4 用例"）但不阻塞。理由：未来 part 4 多项目扩展时减少摩擦；LLM 在大型项目判读可用度通常足够 first pass。

### 10.4 `source_hash` 格式

**单文件 case**：
```
source_hash = "sha256:abc123def456..."
```
- 算法：SHA-256
- 输入：`ground_truth` 字段指向的单文件**字节内容**
- 输出：64 个 hex 字符（小写）

**多文件 case**（part 4）：
```
source_hash = "merkle:sha256:def456..."
```
- 算法：Merkle root over SHA-256
- 输入：`ground_truth` 目录下所有文件（按相对路径排序后逐个 hash → 拼成一条字节流再 hash）
- 输出：64 个 hex 字符（小写）

校验：strip_acsl 跑前 + validate_manifest 校验时都重算 hash，mismatch 报错。

### 10.5 人工 override 机制

跟 §7.6 difficulty 同样的机制。覆盖后：
```toml
[variants.strip_policy.classified_by]
type = "human-override"
reviewed_by_human = true
classified_at = "2026-05-13"

[variants.strip_policy.overridden_from]
goal_type = "rte_only"
strip_lines = [...]
[variants.strip_policy.overridden_from.classified_by]
type = "llm"
# ...
```

---

## 11. `[project]` 段（仅 part 4）

```toml
[project]
upstream_repo                = "https://..."
upstream_commit              = "..."
build_system                 = "<词表见 §15.10>"
loc                          = int
function_count               = int
upstream_verification_effort = "..."
verification_completeness    = "full" | "partial" | "initial"
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `upstream_repo` | str | ✓ | submodule 引用的 git URL |
| `upstream_commit` | str | ✓ | submodule 精确 commit pin |
| `build_system` | str | ✓ | 词表见 §15.10 |
| `loc` | int | ✓ | 源码行数（不含注释和空行）|
| `function_count` | int | ✓ | 函数总数 |
| `upstream_verification_effort` | str |   | 上游 verification 工作的简述 |
| `verification_completeness` | str | ✓ | `full` / `partial` / `initial` |

---

## 12. `[transformations]` 段（仅 part 4）

```toml
[transformations]
sequence = ["01-flatten-includes.sh", "02-merge-headers.patch"]
notes    = ""
```

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `sequence` | list[str] | ✓ | 应用顺序，文件名相对 `transformations/` 目录；空 list 表示无 transformation |
| `notes` | str |   | 自由文本 |

**transformations 的语义边界**：只做**verifier-中立**的预处理（如展平 include、合并多文件、规范化预处理器宏）。**不**做 Frama-C 版本适配等 consumer 责任的事。

---

## 13. `[mined]` 段（仅 part 5）

```toml
[mined]
source_dataset       = "casp" | "fm_bench_acsl"
upstream_id          = "..."
ultimate_origin      = "the_stack_v2" | "the_stack_v1" | ""
ultimate_origin_path = ""
per_file_license     = ""

[mined.casp]                                  # 仅 source_dataset=casp 必填
upstream_verified    = true
total_goals          = 11
verified_goals       = 11
error_cause          = ""

[mined.fm_bench]                              # 仅 source_dataset=fm_bench_acsl 必填
task                 = "Code2Proof"           # 或 SegGen / ProofGen / ReqAna / ProofInfill
folder               = "find___find"
uid                  = "f2d9414c-..."
```

### 13.1 公共字段

| 字段 | 类型 | 必填 | 约束 |
|---|---|---|---|
| `source_dataset` | str | ✓ | 词表见 §15.11 |
| `upstream_id` | str | ✓ | dataset 内的 case 标识。CASP 是 `file_name`；FM-Bench 是 `uid` 或 `folder` |
| `ultimate_origin` | str |   | mined 的原始来源；CASP 衍生自 The Stack，填 `the_stack_v2`；空值合法 |
| `ultimate_origin_path` | str |   | 原始来源中的路径；CASP 不提供，填空 |
| `per_file_license` | str |   | 该 case 在 ultimate_origin 中的许可（SPDX）；用户可查询后填 |

### 13.2 `[mined.casp]` 子段（仅 CASP）

将 CASP 上游 parquet 的特有字段映射进 case.toml，使 consumer 不必读 `_index.toml` 也能获得这些信息：

| 字段 | 类型 | 必填 | 来源 |
|---|---|---|---|
| `upstream_verified` | bool | ✓ | parquet `verified` 列 |
| `total_goals` | int | ✓ | parquet `total_goals` 列 |
| `verified_goals` | int | ✓ | parquet `verified_goals` 列 |
| `error_cause` | str | ✓（可空字符串）| parquet `error_cause` 列 |

注：上游 `verified=True` 时 `total_goals == verified_goals`；`verified=False` 时 `verified_goals < total_goals`。CASP 当前 506 个全部 `verified=True`。

### 13.3 `[mined.fm_bench]` 子段（仅 FM-Bench ACSL slice）

详细字段定义待 §12.0 POC（fetch_fm_bench.py 实现）后再定。占位字段：

| 字段 | 类型 | 必填 | 来源 |
|---|---|---|---|
| `task` | str | ✓ | jsonl `task` 字段（ReqAna / SegGen / ProofGen / ProofComp / ProofInfill / Code2Proof）|
| `folder` | str | ✓ | jsonl `folder` 字段 |
| `uid` | str | ✓ | jsonl `uid` 字段 |

---

## 14. 段间互斥与必填约束总览

| 段 | part 1 | part 2 | part 3 | part 4 | part 5 |
|---|---|---|---|---|---|
| `schema_version` / `id` / `name` / `part` | ✓ | ✓ | ✓ | ✓ | ✓ |
| `[provenance]` | ✓ | ✓ | ✓ | ✓ | ✓ |
| `[files]` | ✓ | ✓ | ✓ | ✓ | ✓ |
| `features` | ✓ | ✓ | ✓ | ✓ | ✓ |
| `[difficulty]` + `[difficulty.scored_by]` | ✓ | ✓ | ✓ | ✓ | ✓ |
| `[parse]` | ✓ | ✓ | ✗ | ✗ | ✗ |
| `[verification]` | ✗ | ✗ | ✓ | ✓ | ✓ |
| `[variants]` + `[variants.strip_policy]` + `[variants.strip_policy.classified_by]` | ✗ | ✗ | ✓ | ✓ | ✓ |
| `[project]` | ✗ | ✗ | ✗ | ✓ | ✗ |
| `[transformations]` | ✗ | ✗ | ✗ | ✓ | ✗ |
| `[mined]` | ✗ | ✗ | ✗ | ✗ | ✓ |
| `[mined.casp]` | ✗ | ✗ | ✗ | ✗ | source_dataset=casp 必填 |
| `[mined.fm_bench]` | ✗ | ✗ | ✗ | ✗ | source_dataset=fm_bench_acsl 必填 |

✓ = 必填  ✗ = 不允许出现

---

## 15. 词表权威清单

### 15.1 `source` 词表

| 值 | 上游 |
|---|---|
| `goblintcil` | github.com/goblint/cil（test/） |
| `frama_c_syntax` | Frama-C tests/syntax |
| `frama_c_cil` | Frama-C tests/cil |
| `frama_c_wp_acsl` | Frama-C wp_acsl |
| `frama_c_spec` | Frama-C tests/spec |
| `frama_c_wp_gallery` | Frama-C wp_gallery |
| `frama_c_wp_models` | Frama-C wp_hoare/typed/bytes/region/tip |
| `acsl_by_example` | github.com/fraunhoferfokus/acsl-by-example |
| `anssi_x509_parser` | github.com/ANSSI-FR/x509-parser |
| `casp` | huggingface.co/datasets/nicher92/CASP_dataset |
| `fm_bench_acsl` | huggingface.co/datasets/fm-universe/FM-bench (ACSL slice) |
| `inhouse` | 本仓库自维护 |
| `inhouse_l1_pure` ... `inhouse_l6_callers` | 来自原 fv-core/tests/benchmark/verification/L1-L6 |

新增 source 须先 PR 提议到 `docs/manifest-schema.md` + `NOTICE`。

### 15.2 `quality` 词表

| 值 | 含义 |
|---|---|
| `algorithm` | 真实算法（gallery 类）|
| `textbook` | 教科书示例（ACSL by Example 类）|
| `acsl_feature_demo` | ACSL 特性演示（wp_acsl 类）|
| `wp_internal_regression` | WP 内部回归测试（wp_hoare/typed/bytes/region/tip 类）|
| `synthetic_corner` | 人造边界 case |
| `frontend_stress` | 前端压力 case（GoblintCil 类）|

作用域：跨 part。

### 15.3 `intent` 词表

| 值 | 含义 |
|---|---|
| `functional_correctness` | 测试功能正确性证明 |
| `feature_demo` | 演示某 C/ACSL 特性如何使用 |
| `parser_stress` | 测试 parser 边界 |
| `edge_case` | 算法边界条件 |
| `proof_technique` | 演示某证明技巧（induction / tactic / ghost） |
| `memory_model_stress` | 测试内存模型边界（wp_typed/bytes 类）|

作用域：跨 part。

### 15.4 `features` 词表（24 项，定义）

#### 数据相关（8 项）

| 值 | 含义 |
|---|---|
| `pure_arith` | 纯算术运算，无内存访问、无指针、无数组 |
| `arrays_ro` | 只读访问数组（读不写），无 mutation |
| `arrays_mut` | 修改数组元素（写或读写）|
| `pointers` | 使用 C 指针（任何用法）|
| `pointer_arithmetic` | 指针算术（`p+i` / `p++` / 指针比较等），非单纯解引用 |
| `linked_struct` | 链表 / 树 / 图等递归数据结构 |
| `nested_struct` | 嵌套 struct（struct 字段是 struct / union）|
| `strings` | C 字符串处理（`strlen` / `strcpy` 等等价语义）|

#### 控制流（4 项）

| 值 | 含义 |
|---|---|
| `loops` | 含至少一个 loop（for / while / do-while）|
| `nested_loops` | 含嵌套 loop（≥2 层）|
| `recursion` | 函数递归调用自己 |
| `callers` | 当前 case 是 caller，调用一个或多个 helper 函数 |

#### 数值（5 项）

| 值 | 含义 |
|---|---|
| `bitwise` | 位运算（`&` / `|` / `^` / `~` / `<<` / `>>`）作为关键逻辑 |
| `floats` | 浮点数运算（`float` / `double`）|
| `overflow_concern` | 整数溢出是验证目标之一（含 RTE-style overflow check）|
| `linear_arith` | 仅需线性算术理论可证 |
| `nonlinear_arith` | 需非线性算术（乘除 over 非常量等）|

#### ACSL 构造（7 项）

| 值 | 含义 |
|---|---|
| `ghost` | 含 ACSL ghost code（`//@ ghost ...` / `/*@ ghost { ... } */`）|
| `axiomatic` | 含 `axiomatic { ... }` 块 |
| `logic_function` | 含 `logic` 函数定义 |
| `predicate` | 含 `predicate` 定义 |
| `lemma` | 含 `lemma` 子句 |
| `behaviors` | 含 `behavior` / `disjoint behaviors` / `complete behaviors` |
| `termination` | 含 `decreases` 子句 或 `terminates` 子句 |

### 15.5 `auxiliary_needed` 词表

| 值 | 含义 |
|---|---|
| `loop_invariant` | 需要 loop invariant |
| `predicate_def` | 需要用户定义 predicate |
| `lemma` | 需要 lemma 辅助 |
| `ghost` | 需要 ghost code |
| `induction` | 需要 induction 证明 |

### 15.6 `classified_by.type` / `scored_by.type` 词表

| 值 | 含义 |
|---|---|
| `llm` | LLM 自动判读，未经人工 review |
| `human` | 人工填写，初次评分 |
| `upstream_split` | 来自上游 dataset 已有的 split（如 CASP）|
| `human-override` | LLM 自动判读后被人工覆盖 |

### 15.7 `constructs_used` 词表（含语法例）

| 值 | 含义 | 语法例 |
|---|---|---|
| `gcc_extension` | GCC 扩展 | `int x __attribute__((aligned(16)));`、`__alignof__(int)`、`__typeof__(x)` |
| `msvc_extension` | MSVC 扩展 | `__declspec(...)` |
| `acsl_extension` | 非标准 ACSL 扩展（WP 私有等）| `/*@ region PQ: *p, *q; */`、`strategy: ...`、`proof: ...` |
| `c99_only` | C99 才引入 | `// 行注释`（虽 C99 后通用）、`_Bool`、复合字面量 `(int[]){1,2,3}` |
| `c11_only` | C11 才引入 | `_Static_assert`、`_Generic`、`_Atomic`、`_Thread_local` |
| `c17_only` | C17 才引入 | 主要是 defect fix，新构造少；用 `__STDC_VERSION__ >= 201710L` |
| `vla` | 变长数组 | `int a[n];`（n 是运行时变量）|
| `designated_initializer` | 指定初始化器 | `int a[5] = {[2] = 7};`、`struct S s = {.x = 1};` |
| `bitfield` | 位域 | `struct { unsigned x : 3; } s;` |
| `_Alignas` | `_Alignas` 关键字 | `_Alignas(16) char buf[64];` |
| `_Generic` | `_Generic` 关键字 | `_Generic((x), int: 1, default: 0)` |
| `statement_expression` | GCC 语句表达式 | `int x = ({int t = a; t * t;});` |

新增 construct 词须 PR 到本节。

### 15.8 `verification.target` 词表

见 §9.1。

### 15.9 `goal_type` 词表

| 值 | 含义 |
|---|---|
| `rte_only` | 没有显式 ACSL goal，目标是 WP+RTE 全过；stripped 通常是纯 C |
| `explicit_assert` | 有显式顶层或函数体 `assert` 作为最终目标 |
| `explicit_lemma` | 有显式 `lemma` 子句作为最终结论 |
| `mixed` | 混合：既有 assert 又有 lemma 等 |

### 15.10 `build_system` 词表

| 值 | 含义 |
|---|---|
| `make` | Makefile |
| `cmake` | CMake |
| `dune` | OCaml Dune（C 项目少见但保留）|
| `none` | 无 build system，单独编译 |

### 15.11 `mined.source_dataset` 词表

| 值 |
|---|
| `casp` |
| `fm_bench_acsl` |

新增 mined dataset 须 PR。

---

## 16. 校验规则（`validate_manifest.py` 检查项）

### 16.1 字段级

- `schema_version` 严格等于 `tools/schema_version.py` 常量
- 必填字段存在且类型正确
- 词表字段值在合法集合内
- 整数字段在合法区间（如 `difficulty.*` 必须 0–5）
- 日期字段（`scored_at` / `classified_at`）符合 ISO 8601 日期形态（`YYYY-MM-DD`）

### 16.2 字段间约束

- `id` 等于 case.toml 所在目录相对 `cases/` 的路径
- `part` 值与 id 前缀一致：part=1 → id 以 `part1_c_features/` 开头，等等
- §14 表格的"段必填/不允许"逐 part 校验
- `scored_by.type == "llm"` ⇒ `model` 和 `prompt_version` 必填
- `classified_by.type == "llm"` 同上
- `target` 中 `total_correctness` 与 `functional` 不应同时出现（前者已蕴含后者）—— 仅 warning
- **Part 4 case 的 `classified_by.reviewed_by_human` 建议 true**（v2 软化：false 仅 warning，不阻塞）
- Part 5 case 的 `source_dataset == "casp"` ⇒ `[mined.casp]` 段必填
- Part 5 case 的 `source_dataset == "fm_bench_acsl"` ⇒ `[mined.fm_bench]` 段必填

### 16.3 文件存在性（`--check-files` 模式）

- `entry_files` 路径存在
- `[variants].stripped` 路径存在
- `[variants].ground_truth` 若填则路径存在
- `[license_file]` 指向的 LICENSES/ 下文件存在
- 上游 commit 不验证（不要求联网）

### 16.4 内容一致性（`--check-strip-hash` 模式）

- `[variants.strip_policy].source_hash` 与 `ground_truth` 实际内容 hash 匹配

---

## 17. 顶层与分片 Manifest

### 17.1 `manifest.toml`（顶层）

```toml
schema_version = "0.1"
bench_version  = "0.1.0"                      # SemVer
generated_at   = "2026-05-13T..."             # ISO 8601 datetime

[parts.part1_c_features]
manifest = "manifest_part1.toml"
case_count = int

[parts.part2_acsl_features]
manifest = "manifest_part2.toml"
case_count = int

# ... 5 个 part 都列
```

字段：

| 字段 | 类型 | 必填 |
|---|---|---|
| `schema_version` | str | ✓ |
| `bench_version` | str | ✓ |
| `generated_at` | str | ✓ |
| `parts.<part_dir>.manifest` | str | ✓ |
| `parts.<part_dir>.case_count` | int | ✓ |

### 17.2 `manifest_partN.toml`（分片）

```toml
[[case]]
id                 = "..."
path               = "cases/..."
source             = "..."
license            = "..."
quality            = "..."
intent             = "..."
difficulty_overall = int
features           = [...]
has_ground_truth   = bool
has_stripped       = bool

[[case]]
# ... 下一个
```

字段（每 case）：

| 字段 | 类型 | 来源 |
|---|---|---|
| `id` | str | case.toml.id |
| `path` | str | 相对仓库根 |
| `source` | str | case.toml.[provenance].source |
| `license` | str | case.toml.[provenance].license |
| `quality` | str | case.toml.[provenance].quality |
| `intent` | str | case.toml.[provenance].intent |
| `difficulty_overall` | int | case.toml.[difficulty].overall |
| `features` | list[str] | case.toml.features |
| `has_ground_truth` | bool | case.toml.[variants].ground_truth 是否非空 |
| `has_stripped` | bool | case.toml.[variants].stripped 是否非空 |

注：其他字段不缓存到分片；consumer 需要更多时读对应 case.toml。

---

## 18. 特殊 `_index.toml` Schema

### 18.1 CASP `_index.toml`

位置：`cases/part5_mined/casp/_index.toml`

作用：CASP 不重分发，但保留 506 个 case 的**精简骨架**在版本库内（供 `fetch_casp.py` 决定要生成哪些 case + 不依赖 fetch 也能跑 build_views）。

**注意（v2 修订）**：CASP 特有元数据（total_goals 等）现在**同时**写入每个 case.toml 的 `[mined.casp]` 段。`_index.toml` 只作为 fetch 入口和 case 列表存在，不再是该数据的 source of truth。

```toml
schema_version = "0.1"
source_dataset = "casp"
upstream_url   = "https://huggingface.co/datasets/nicher92/CASP_dataset"
upstream_commit_hash = "<HF dataset SHA / sha256 of train.parquet>"

[[case]]
id          = "part5_mined/casp/00001"
upstream_id = "frama_c_files_v2/test_frama_c_2054.c"

[[case]]
# ... 共 506 个
```

字段：

| 字段 | 类型 | 必填 |
|---|---|---|
| `schema_version` | str | ✓ |
| `source_dataset` | str | ✓ |
| `upstream_url` | str | ✓ |
| `upstream_commit_hash` | str | ✓ |
| `case[].id` | str | ✓ |
| `case[].upstream_id` | str | ✓ |

`fetch_casp.py` 流程：
1. 读 `_index.toml` 获取要生成的 case 列表（id + upstream_id）
2. 下载/解码 parquet
3. 为每个 case 生成 `case.toml`（含 `[mined.casp]` 段填上游 verified/total_goals/verified_goals/error_cause）+ `ground_truth.c` + `stripped.c`

### 18.2 FM-Bench `_index.toml`

结构待定，§12.0 POC 之后再补。当前留空。

---

## 19. Schema 版本升级策略

### 19.1 SemVer

`schema_version` 用三段式 SemVer：

| 升级类型 | 触发条件 |
|---|---|
| Major（`1.0` → `2.0`）| 不兼容修改：删字段、改字段类型、改语义 |
| Minor（`0.1` → `0.2`）| 新增可选字段、新增词表项 |
| Patch（`0.1.0` → `0.1.1`）| 文档纠错、措辞调整，不影响 case.toml 内容 |

注：Patch 级别不写到 case.toml 字段；case.toml 的 `schema_version` 只写到 Minor。

### 19.2 升级流程

1. 修改本文档 + bump `tools/schema_version.py` 的常量
2. 修改 `tools/validate_manifest.py` 以匹配新 schema
3. 实现 `tools/migrate_schema.py` 处理 vN → vN+1 转换
4. 跑 `migrate_schema.py` 把所有 case.toml 升级
5. 跑 `validate_manifest.py` 全量校验
6. PR 包含：本文档变更 + version 常量 + migrate 脚本 + 全量 case.toml 升级

### 19.3 Migrate 工具

`tools/migrate_schema.py` 需要实现（**后续工作**）：

- 自动检测 case.toml 当前版本
- 链式升级（0.1 → 0.2 → 0.3 → ...）
- 不可自动升级的情况（如新增必填字段无默认值）→ 标记并报告，要求人工填充
- Dry-run 模式

### 19.4 Schema 不兼容修改的过渡期

- Major 升级前：发 RFC 至少 2 周
- 提供 `tools/migrate_schema.py --target <ver>` 工具
- 旧 schema 的 case.toml 保留在 git history 中作为参考；不主动维护两套
- consumer 期望同步升级（无 polyglot 期）

---

## 20. 后续工作清单（本文档未尽事项）

- §15.7 `constructs_used` 词表的扩展（其他 GCC/MSVC 扩展）
- §18.2 FM-Bench `_index.toml` schema
- 实现 `tools/migrate_schema.py`
- `docs/per-part/*.md` 中各 part 的字段实际取值范例

---

## 附录 A：完整 case.toml 模板

### Part 3 短小算法 case 模板

```toml
schema_version = "0.1"
id            = "part3_curated/acsl_by_example/binary_search"
name          = "binary_search"
part          = 3

[provenance]
source             = "acsl_by_example"
upstream_url       = "https://github.com/fraunhoferfokus/acsl-by-example"
upstream_path      = "StandardAlgorithms/BinarySearch/binary_search.c"
upstream_commit    = "<pin>"
upstream_tool_hint = "frama-c 32 (Gallium)"
license            = "MIT"
license_file       = "LICENSES/MIT-fraunhofer-fokus.txt"
quality            = "textbook"
intent             = "functional_correctness"

[files]
entry_files = ["binary_search.c"]

features = ["loops", "arrays_ro", "linear_arith"]

[difficulty]
overall            = 3
proof_size         = 1
math_theory        = 2
data_complexity    = 2
control_complexity = 2
spec_complexity    = 1
auxiliary_needed   = ["loop_invariant"]

[difficulty.scored_by]
type = "llm"
model = "claude-opus-4-7"
prompt_version = "v0.1"
scored_at = "2026-05-13"

[verification]
target = ["functional"]

[variants]
ground_truth = "variants/ground_truth/binary_search.c"
stripped     = "variants/stripped/binary_search.c"

[variants.strip_policy]
goal_type = "explicit_assert"
goal_lines = [5, 6, 7]                        # ensures Result / Found / NotFound
strip_lines = [10, 15]                        # loop invariants
source_hash = "sha256:..."
rationale = "..."

[variants.strip_policy.classified_by]
type = "llm"
model = "claude-opus-4-7"
prompt_version = "v0.1"
reviewed_by_human = false
classified_at = "2026-05-13"
```

### Part 5 CASP case 模板

```toml
schema_version = "0.1"
id   = "part5_mined/casp/00042-clamp"
name = "clamp"
part = 5

[provenance]
source             = "casp"
upstream_url       = "https://huggingface.co/datasets/nicher92/CASP_dataset"
upstream_path      = "frama_c_files_v2/test_frama_c_2054.c"
upstream_tool_hint = "frama-c (version unknown)"
license            = "NOASSERTION"
license_file       = "LICENSES/CASP-attribution.txt"
quality            = "algorithm"
intent             = "functional_correctness"

[files]
entry_files = ["ground_truth.c"]              # CASP case 物理文件名约定

features = ["pure_arith"]

[difficulty]
overall            = 1
proof_size         = 0
math_theory        = 1
data_complexity    = 1
control_complexity = 0
spec_complexity    = 1
auxiliary_needed   = []

[difficulty.scored_by]
type = "llm"
model = "claude-opus-4-7"
prompt_version = "v0.1"
scored_at = "2026-05-13"

[verification]
target = ["functional"]

[variants]
ground_truth = "ground_truth.c"
stripped     = "stripped.c"

[variants.strip_policy]
goal_type   = "rte_only"
goal_lines  = []
strip_lines = [1]
source_hash = "sha256:..."
rationale   = "CASP 上游 split，无显式 goal。"

[variants.strip_policy.classified_by]
type = "upstream_split"
reviewed_by_human = false
classified_at = "2026-05-13"

[mined]
source_dataset       = "casp"
upstream_id          = "frama_c_files_v2/test_frama_c_2054.c"
ultimate_origin      = "the_stack_v2"
ultimate_origin_path = ""
per_file_license     = ""

[mined.casp]
upstream_verified = true
total_goals       = 1
verified_goals    = 1
error_cause       = ""
```
