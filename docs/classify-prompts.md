# Classify-Case Prompts

> Prompt 版本管理 + 设计说明。`classify_case.py` 用这些 prompt 调 LLM 判读
> case.toml 的 `[variants.strip_policy]` + `[difficulty]` 段。
>
> 任何 prompt 改动 → bump version → 写到 case.toml 的 `prompt_version` 字段。
> 同 case 用不同 prompt_version 跑出的结果不可互相替换；重跑前要 reclassify。

---

## 1. 当前活跃版本

- **`v0.1`** — initial difficulty + goal_type 评分（本文档 §2）

---

## 2. Prompt `v0.1`

### 2.1 适用范围

- Part 3 / 4 / 5 用例（功能验证类，有 `[variants]` 段）
- 输入：ground_truth.c 内容 + 当前 case.toml 的 part / features / target
- 输出：difficulty 6 维度评分 + goal_type 检测 + rationale

不适用：
- Part 1 / 2 用例（前端覆盖类，没有 difficulty/strip_policy 需求）—— skip
- ground_truth 超过 LLM context window（约 100K tokens）的用例 —— 用 truncated 提示或 split

### 2.2 System prompt

```
You are an expert in C program verification with ACSL (ANSI/ISO C Specification
Language). Your task is to analyze a C+ACSL source file from a verification
benchmark and produce structured metadata about it.

You will receive:
  - The full source content (C code + ACSL annotations)
  - Some context (part number, declared features, declared verification target)

You output a single JSON object with the schema specified by the user. Do not
output any explanation outside the JSON. Use the user's exact field names.

Be calibrated: most cases are 1-3 in difficulty; reserve 4-5 for genuinely
complex cases (induction, custom theories, multi-file projects, 100+ ACSL
annotations).
```

### 2.3 User prompt template

```
Analyze this C+ACSL case from the FermatVerification-bench. Output JSON with
fields below.

CASE CONTEXT:
  part:     {part}
  features: {features_list}
  target:   {verification_target}
  source:   {provenance_source}
  file size: {byte_count} bytes / {line_count} lines

SOURCE CONTENT (ground_truth):
```
{ground_truth_content}
```

REQUIRED OUTPUT JSON:

{{
  "goal_type": "rte_only" | "explicit_assert" | "explicit_lemma" | "mixed",
  "goal_lines": [<line numbers of ACSL blocks that ARE the verification goal>],
  "rationale": "<1-3 sentence explanation>",
  "difficulty": {{
    "overall": 0-5,
    "proof_size": 0-5,
    "math_theory": 0-5,
    "data_complexity": 0-5,
    "control_complexity": 0-5,
    "spec_complexity": 0-5
  }},
  "auxiliary_needed": [<词表: loop_invariant, predicate_def, lemma, ghost, induction>]
}}

DEFINITIONS (see docs/manifest-schema.md §7.3 for full anchors):

- goal_type:
  * rte_only       — 没有显式 ACSL goal；目标是 WP+RTE 全过
  * explicit_assert — 有顶层或函数体 `assert` 作为最终目标
  * explicit_lemma  — 有显式 `lemma` 子句作为最终结论
  * mixed           — 上述混合

- 函数 pre/post (requires/ensures/assigns) 是**过程规约**，不是 goal。
  goal 是 ULTIMATE 目标。只有明显非辅助性的 assert / lemma 才算 goal。

- goal_lines: 列出每个 goal-性质 ACSL 块的起始行号（1-indexed）。
  rte_only 时通常返回 []。

- difficulty 0-5 (see anchors in manifest-schema.md §7.3):
  * proof_size: ACSL 块数级别 (0=0-2 / 1=3-10 / 2=11-30 / 3=31-100 / 4=101-300 / 5=>300)
  * math_theory: 0=布尔 / 1=Presburger / 2=线性+数组 / 3=非线性/bv / 4=混合 / 5=自定义
  * data_complexity: 0=纯函数 / 1=scalar / 2=array / 3=指针/struct / 4=linked/tree / 5=复合堆
  * control_complexity: 0=直线 / 1=分支 / 2=单循环 / 3=嵌套 / 4=递归 / 5=互递归
  * spec_complexity: 0=无ACSL / 1=pre/post / 2=behaviors / 3=ghost/axiomatic / 4=lemma / 5=induction
  * overall: 综合（不是聚合公式，独立判断）

- auxiliary_needed: 验证此 case 通常需要 agent 额外引入的辅助构件类型。

OUTPUT JSON ONLY — no surrounding text, no markdown, no code fences.
```

### 2.4 输出 schema 校验

`classify_case.py` 收到 LLM 响应后：

1. JSON parse — 失败 → 报错 + 保留原 case.toml
2. 字段完整性校验：
   - `goal_type` 在词表 §15.9
   - `goal_lines` 是 list[int]
   - `difficulty.*` 6 字段都在 0-5
   - `auxiliary_needed` 元素在词表 §15.5
   - `rationale` 非空字符串
3. 通过 → 更新 case.toml 的 `[variants.strip_policy]` 和 `[difficulty]` 段
4. 失败任一项 → 保留原 case.toml，输出错误细节

### 2.5 已知局限

- **大文件**：ground_truth > 50K tokens 时，prompt 超出 context window。
  当前策略：truncate + 头尾保留（中间用 `[... truncated ...]`）。理想的策略是
  Map-Reduce（分段评分 + 聚合）—— 未实现，v0.2.x 工作。
- **LLM 调用成本**：1933 case 全跑一次 = 约 $20-50（按 claude-opus-4-7 估）。
  建议先在 small subset 跑（如 part 5 CASP 506 中抽 50）验证 prompt 质量。
- **重复评分一致性**：同一 case 跑两次可能差 1-2 档。建议加 `temperature=0.1` 或
  跑多次取中位数。当前 prompt 用 `temperature=0`。

---

## 3. 历史版本

（暂无）

未来 prompt 升级（如 v0.2）应在此节记录，并提供 v0.1 → v0.2 的迁移说明。

---

## 4. 测试与回归

- `tools/tests/test_classify_prompts.py`（待实现）：
  - dry-run prompt 渲染正确性
  - 输出 JSON 解析合法性
  - 词表合规性

- Fixture cases（待选）：每 part 1-2 个代表 case，固定 expected output
  (从 manual review 得来) 作为 oracle。

---

## 5. 词表权威

所有词表 ID 与本文档冲突时，以 `docs/manifest-schema.md` §15 为准。

任何新增 goal_type / difficulty 维度 / auxiliary_needed 词，须先在 schema 文档
+ vocab.py 通过，再更新本 prompt。
