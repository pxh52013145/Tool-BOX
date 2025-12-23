# Password Manager 模块（已落地）

本模块对应任务书题目「个人密码管理器（数据库 + 加密 + 文件操作）」的核心功能：账号密码安全存储、加密保护、按类别过滤、加密备份导入导出。

## 1. 已实现功能
- Vault：首次设置主密码；解锁/锁定；5 分钟无操作自动锁定；修改主密码会对所有条目重新加密。
- 条目管理：新增/编辑/删除；搜索；按分类过滤；复制账号/复制密码（复制密码二次确认，15 秒后自动清空剪贴板）。
- 数据持久化：SQLite 存储；密码与备注字段加密后写入数据库。
- 备份/恢复：导出/导入加密备份文件（`.tbxpm`）。

## 2. 关键 Qt 模块覆盖
- 数据库：`QtSql` + SQLite（增删改查）。
- Model/View：`QTableView` + 自定义 `QAbstractTableModel` + `QSortFilterProxyModel`。
- 文件操作：备份文件的读写（`QFileDialog` / `QSaveFile` / JSON）。

## 3. 数据库设计
- `vault_meta`：保存 KDF 参数与主密码校验值（`id=1` 单行）。
- `password_entries`：保存条目基本信息（明文）与机密字段（密文）：
  - 明文：`title/username/url/category`、时间戳
  - 密文：`password_enc/notes_enc`

## 4. 加密设计（课程项目落地版）
- KDF：PBKDF2-SHA256（默认 `iterations=120000`，`salt=16 bytes`，`key=32 bytes`）。
- 加密/校验：基于 HMAC-SHA256 生成密钥流（异或加密）+ HMAC 校验（tag 截断为 16 bytes）。
- 备注：这是课程设计的实现方案，目的是满足“加密存储 + 可演示”的要求，并非专业密码学库的替代品。

## 5. 代码位置
- Vault/加密：`src/password/passwordvault.*`、`src/core/crypto.*`
- 数据访问：`src/password/passwordrepository.*`
- 列表模型：`src/password/passwordentrymodel.*`
- UI：`src/pages/passwordmanagerpage.*`、`src/pages/passwordentrydialog.*`
