# `tools/` — bench 仓库工具

详见 `docs/architecture.md` §7 工具体系一节。

## 当前组件

| 文件 | 用途 |
|---|---|
| `schema_version.py` | `SCHEMA_VERSION` 常量单一来源 |
| `strip_acsl.py` | Layer 1: 机械删除指定行的 ACSL 块（待实现）|
| `classify_case.py` | Layer 2: LLM 判读 strip_policy + difficulty（待实现）|
| `validate_manifest.py` | case.toml schema 校验（待实现）|
| `build_views.py` | 聚合 case.toml 生成顶层 manifest + 分片 + views（待实现）|
| `fetch_casp.py` | 从 HuggingFace 下载 CASP + 生成 part5 case.toml（待实现）|
| `fetch_fm_bench.py` | 从 HuggingFace 下载 FM-Bench ACSL 切片（待实现）|

## 子目录

- `lib/` — 共享模块
- `tests/` — pytest 测试 + fixtures
