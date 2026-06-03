# FermatVerification-bench

> 统一的 **C + ACSL 验证基准库**，verifier-agnostic 设计，分 5 部分覆盖前端、规约、算法、真实项目、mined corpus。提供 C+ACSL 制品作为验证基准，不绑定具体 verifier；verifier 结果由 consumer 外置管理，不进本仓库。

## 项目结构

```
cases/                  — 全部用例，按 5 部分分目录，每个 case 一个目录 + case.toml
  part1_c_features/     — Part 1: C 语言特性（差分 frontend）
  part2_acsl_features/  — Part 2: ACSL 语言特性
  part3_curated/        — Part 3: 短小算法（人工挑选，双版本）
  part4_real_targets/   — Part 4: 真实验证目标项目（多文件 + build system）
  part5_mined/          — Part 5: mined 外部 corpus（按数据集分目录）
tools/                  — Python 工具体系（ingest / strip / validate / build views）
  lib/                  — 共享模块（vocab.py 等权威词表）
  tests/                — pytest 测试 + fixtures
docs/                   — 设计文档与流程规范
  architecture.md       — 总体架构、5 部分定位、设计决策
  manifest-schema.md    — case.toml 字段权威定义、词表、难度标度、校验规则
  classify-prompts.md   — LLM classifier prompt 设计
  workflow.md           — 开发工作流程规范（@docs/workflow.md）
manifest.toml           — 顶层 bench 元数据 + 5 个分片引用
manifest_part{1..5}.toml — 按 part 分片的 case 索引（由 build_views.py 生成）
views/                  — 聚合统计 markdown（由 build_views.py 生成）
LICENSES/ NOTICE        — 上游许可追踪（cases 内源码保留各自上游许可）
```

## 技术栈

<!-- 项目初期按已知信息填写，随开发推进逐步补充完善 -->

- 语言：Python 3.12（工具链）；用例制品为 C + ACSL
- 构建系统：无（数据库 + 脚本，非编译型项目）；venv 管理 Python 依赖
- 测试框架：pytest（`tools/tests/`）
- 关键依赖：`pyarrow`（fetch CASP parquet 用）
- 元数据格式：TOML（每个 case 一份 `case.toml`，schema 版本见 `tools/schema_version.py`）

## 常用命令

<!-- 确定构建和测试方式后补充，保持与实际可用命令同步 -->

```bash
# 初始化 venv（首次）
python3 -m venv .venv && .venv/bin/pip install pyarrow

# Fetch CASP 数据（首次必做，.c 文件默认 .gitignore，需脚本生成）
.venv/bin/python tools/fetch_casp.py

# 校验 manifest 正确性（应输出 0 error(s)）
python3 tools/validate_manifest.py --check-files --check-strip-hash

# 修改 cases/ 后重新生成 manifest + views
python3 tools/build_views.py            # 写 manifest.toml + 5 分片 + 5 views
python3 tools/build_views.py --check    # CI 模式：仅校验是否最新

# 运行工具测试
python3 -m pytest tools/tests/
```

## 代码风格

<!-- 可在首次代码审核后根据实际约定补充，初期留空不影响使用 -->

- 命名：Python 用 snake_case；case id 用 `part{N}_{name}/{case_name}` 形式
- 注释语言：中文
- 文档语言：中文（设计文档、commit message、devlog 均用中文）
- case.toml 字段语义以 `docs/manifest-schema.md` 为权威定义，词表以 `tools/lib/vocab.py` 为单一来源

## 参考实现

<!-- 列出本项目对标的成熟实现，用于调研对比、算法验证和 bug 修复时的交叉检验（见 workflow.md §2.1, §4.3, §5.1） -->

- **Frama-C / WP**：ACSL 规约的事实标准 verifier，用例语义对标其 wp_gallery / wp_acsl / tests 套件
- **ACSL by Example**（fraunhoferfokus）：算法类用例的规约写法参考
- **CASP**（arXiv:2508.18798）：Part 5 mined corpus 上游
- **FM-Bench**（arXiv:2501.16207）：ACSL slice 来源（待接入）
- **父项目 FermatVerification / fv-core**：inhouse 用例来源，本 bench 是其 submodule

## 已知限制与注意事项

<!-- 记录项目特有的坑、Claude 容易犯的错、隐含约束等无法从代码推断的信息，随开发持续补充 -->

- **CASP / FM-Bench 的 `ground_truth.c` 和 `stripped.c` 默认被 .gitignore**：不重分发，需跑 fetch 脚本生成；per-file 上游许可需用户自查
- **许可边界**：工具代码 + 元数据 + 文档为 MIT；`cases/**/` 内源码保留各自上游许可，新增用例必须正确填 `[provenance].license` 并更新 `NOTICE` / `LICENSES/`
- **manifest_part*.toml 和 views/ 是生成产物**：不要手改，改 `cases/` 后跑 `build_views.py` 重新生成
- **ANSSI x509 (Part 4) 是 git submodule**：clone 后需 `git submodule update --init --recursive`
- **strip 双版本一致性**：part 3/4/5 用例的 `stripped.c` 由 strip 工具从 `ground_truth.c` 机械生成，改动需保证 strip_hash 一致（`validate_manifest.py --check-strip-hash`）
- **task-only 来源**（如 `live_fm_bench`）：held-out 评测集无 gold 解，只产 `stripped.c`、不产 `ground_truth.c`；`source_hash` 绑定 stripped 内容，`§16.4` 检查自动跳过。新增此类 source 时仿 `tools/ingest_live_fm_bench.py`
- **Python 依赖走 `.venv`**：项目根有 `.venv`（已 .gitignore），含 `pip` / `pyarrow` / `pytest`。需要 pyarrow 的工具（`fetch_casp.py`）用 `.venv/bin/python` 跑；`pytest tools/tests/` 用 `.venv/bin/python -m pytest`。系统 `python3` 没有这些包，ingest 类工具应尽量纯 stdlib（用 jsonl 而非 parquet 即可绕开 pyarrow）。注：`.venv` 需 `python3.12-venv` apt 包才能重建（ensurepip）
- **CASP 的 `.c` 默认缺失**：本机没跑 `fetch_casp.py`，故 `validate_manifest.py --check-files` 会报 ~1518 个 CASP 文件不存在错误，属环境态，不是回归。判断自己的改动是否干净时需 `grep -v casp/` 过滤

## 工作流程

遵循 @docs/workflow.md 中定义的开发流程规范。

### 流程速查

```
新功能开发：
  调研 → [确认] → 架构 → [确认] → 细化 → [确认] → 审查 → 逐模块实现+测试+审核

Bug 修复：
  发现错误     → 定位错误 → 构建根因分析文档 (§5.1) → [确认] → fix 分类
  局部错误     → 直接修复 + 补测试 + 回归
  算法内部错误 → 修正方案文档(§5.1) → [确认] → 修复 + 补测试 + 回归
                 ↑ 算法类 bug 必须对照参考实现
  接口/架构问题 → 修正方案文档(§5.1) → [确认] → 更新设计(§2) → 按实现流程执行 + 回归

Claude 每步操作：
  说明计划 → [等待确认] → 执行单步 → 报告结果 → [等待反馈]
```

## 开发日志规范

每完成一个模块或关键里程碑后，在 `docs/devlog/` 下写开发日志。除常规内容（做了什么、为什么、关键决策）外，**必须包含 `## 度量` 段**，记录以下指标：

```markdown
## 度量

| 指标 | 数值 |
|------|------|
| 新增代码行数 | <行数> |
| 修改代码行数 | <行数> |
| 删除代码行数 | <行数> |
| 涉及文件数 | <个数> |
| 新增测试用例数 | <个数> |
| 测试通过率 | <通过数/总数> |
| 发现 bug 数 | <个数> |
| 修复 bug 数 | <个数> |
| 迭代轮次 | <设计/实现/修复各几轮> |
```

- 代码行数通过 `git diff --stat` 获取
- 每条日志文件命名：`docs/devlog/YYYY-MM-DD-<简述>.md`
- 度量不要求精确到个位，合理估算即可，重在趋势追踪
- 日志中总结的**经验教训**（踩过的坑、易错点、有效的做法）须同步回写到本文件的「已知限制与注意事项」段，确保后续开发不重蹈覆辙
