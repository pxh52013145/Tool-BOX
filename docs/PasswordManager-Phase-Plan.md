# 密码管理器对标 KeePassXC：分阶段开发计划（Toolbox）

> 目标：在你现有的“密码管理器（数据库 + 加密 + 文件操作）”基础上，参考 KeePassXC 的模块划分与体验特性，按阶段补齐“组织能力 + 安全可视化 + 互操作 +（可选）网络/浏览器集成”，形成可答辩演示的完整闭环。

## 0. 约束与评分项对齐（课程设计）

- **必须可运行**：Qt Creator 可编译运行。
- **GUI 必须使用布局管理器**（你当前 UI 已使用布局，继续保持）。
- Qt 模块覆盖：你当前已覆盖 **数据库（SQLite/QtSql）+ Model/View + 文件操作**；建议再补齐：
  - **多线程**（KDF/报表扫描/导入导出异步化）
  - **网络通信**（站点图标下载或在线泄露检查）

## 1. 当前基线（你仓库已具备）

来自 `docs/PasswordManage.md` 与代码现状：
- Vault：主密码初始化/解锁/锁定；5 分钟无操作自动锁定；修改主密码会重加密所有条目（`src/password/passwordvault.*`）。
- 条目管理：新增/编辑/删除；搜索；分类过滤；复制账号/复制密码（二次确认 + 15 秒清空剪贴板）（`src/pages/passwordmanagerpage.*`、`src/pages/passwordentrydialog.*`）。
- 持久化：SQLite；密码与备注加密写库（`src/password/passwordrepository.*`、`src/core/database.*`）。
- 备份/恢复：`.tbxpm` 加密备份导入导出（当前逻辑主要在 `src/pages/passwordmanagerpage.*`）。

## 2. 分阶段计划（建议每阶段拆成多次“中文提交”）

### Phase 1：信息组织能力（分组/标签）+ UI 结构升级（对标 KeePassXC 的 Group/Tag）

**范围**
- 新增“分组（树）+ 标签（多选）”能力，让条目从“扁平表”升级为可组织管理。

**关键任务**
- DB：
  - 新增 `groups`（`id`, `parent_id`, `name`, `created_at`, `updated_at`）
  - 新增 `tags`、`entry_tags`（多对多）
  - `password_entries` 增加 `group_id`（默认根组）
- Model/View：
  - `QTreeView + QAbstractItemModel`：分组树
  - `QTableView`：按分组过滤的条目列表（沿用/扩展现有 `PasswordEntryModel`）
  - 标签筛选：可先用 `QSortFilterProxyModel` 组合实现
- UI/交互：
  - 左侧分组树、右侧列表与详情/编辑弹窗
  - 支持把条目移动到分组（可先用“移动到…”菜单，后续再做拖拽）

**验收演示点**
- 新建分组/子分组 → 新建条目归类 → 按分组/标签过滤 → 搜索命中。

### Phase 2：生成器与安全可视化（对标 KeePassXC 的 PasswordGenerator / PasswordHealth）

**范围**
- 把“生成密码”从简单随机串升级为可配置；增加“强度/复用/过期”等可视化指标，形成可演示的安全卖点。

**关键任务**
- 密码生成器：
  - 可配置：长度、字符类（大小写/数字/符号）、排除易混淆字符、每组至少一个
  - 可选加分：口令短语（wordlist）模式
- 强度评估：
  - 基础规则（长度/字符类/常见弱口令）+ 复用检测（同密码多条目）
  - UI：编辑弹窗显示强度条/评分；新增“安全报告”页汇总弱/重复
- 多线程（强烈建议本阶段引入）：
  - 报表扫描/复用检测放到 `QtConcurrent` 或 `QThread`，提供进度与取消（避免 UI 卡顿）

**验收演示点**
- 生成不同策略的密码 → 强度评分变化 → 一键扫描发现重复密码 → 点击定位到条目。

### Phase 3：互操作（CSV 导入导出）+ 备份版本化与迁移

**范围**
- 对标 KeePassXC `format/`：把“备份/互操作”抽成更清晰的模块，并支持常见 CSV 导入导出（浏览器官方导出 CSV 是高性价比入口）。

**关键任务**
- CSV 导入向导：
  - 字段映射（title/username/password/url/notes/category/tags）
  - 预览、去重策略（例如 url+username 或 title+username）
  - 导入过程异步化（大文件不卡 UI）
- CSV/HTML 导出：
  - 解锁后才允许导出；明确风险提示
- `.tbxpm` 备份升级：
  - 增加 `version`、`createdAt`、`appVersion`、KDF 参数等元信息
  - 提供“旧版本备份导入兼容”与“DB schema 迁移记录”

**验收演示点**
- 现场用 Chrome/Edge 导出的 CSV 导入 → 列表中可检索 → 导出 CSV（并提示风险）。

### Phase 4（加分）：网络能力（对标 KeePassXC networking）

**范围**
- 增加可见的网络功能，满足课程“网络通信”模块覆盖，并提升体验。

**关键任务（推荐优先做 favicon 下载）**
- 站点图标：
  - `QNetworkAccessManager` 拉取 `https://domain/favicon.ico` 或解析 `<link rel="icon">`（可先简化为固定 favicon 路径）
  - 本地缓存：DB 或文件系统缓存（带过期时间）
  - UI：列表显示图标/无图标占位
- 可选：泄露检查（HIBP k-Anonymity 思路）或仅做“离线弱口令字典”提示（不依赖网络）

**验收演示点**
- 新增条目带 URL → 自动拉取并显示站点图标；断网时走缓存/降级。

### Phase 5（高难加分，可后置）：浏览器扩展对接（对标 KeePassXC browser/proxy）

**范围**
- 跑通“网页自动填充 + 保存/更新提示”的最小闭环（工程量最大）。

**关键任务**
- 通信架构：
  - 扩展（Chrome/Edge/Firefox）检测登录表单
  - 桌面端提供本地 IPC（建议 `QLocalServer` 或本地 HTTP 仅 `127.0.0.1`）
  - 安全：配对码/一次性 token，避免任意进程读取明文密码
- UI：
  - 站点候选账号列表弹窗、一键填充、提交后保存/更新确认

**验收演示点**
- 录屏演示：打开登录页 → 选择账号自动填充 → 登录后提示保存/更新。

## 3. 每阶段配套文档与记录（课程必须项）

- 架构/流程/数据结构：同步更新 `docs/Framework.md`、`docs/PasswordManage.md`（模块图、流程图、表结构、线程结构）。
- 调试记录：`docs/debug-notes.md`（复现步骤、断点/日志、定位与修复）。
- AI 使用记录：`docs/ai-usage.md` 与 `docs/ai-mistakes.md`（至少 2 例“AI 输出错误/不适用”的复盘）。
- Git 提交：每个子任务一条中文提交（不少于 15 次有效提交；建议 20+）。

