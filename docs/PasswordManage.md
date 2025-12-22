# Password Manager 方案细化（草案）

## 1. MVP 功能清单
- Vault：主密码解锁/锁定、自动锁定、基本设置项。
- 条目管理：新增/编辑/删除登录条目，搜索、标签、最近使用。
- 导入：Chrome/Edge/Firefox 官方 CSV（先覆盖最常见字段）。
- 填充（网页）：浏览器扩展检测登录表单 → 桌面端返回候选账号 → 扩展填充。
- 保存/更新：扩展捕获提交 → 桌面端弹窗确认 → 写入 Vault。

## 2. Vault 设计（建议落地版）
- 文件：`vault.dat`
- Header：magic/version/kdf params/salt
- Payload：加密后的 JSON 或 protobuf（先 JSON，后续可换）
- 加密：
  - KDF：Argon2id（参数可配置，保存到 header）
  - AEAD：XChaCha20-Poly1305（nonce 随机）
- 运行时：
  - 仅在解锁后把明文解密到内存（尽量短生命周期）
  - UI 列表使用脱敏字段（username/origin），密码需要二次动作才解密

## 3. 浏览器扩展对接（推荐协议形态）

### 3.1 消息类型（示例）
- `FormDetected`：`{origin, url, title, fields:{hasUsername, hasPassword}}`
- `QueryLogins`：`{origin}` → 返回 `[{id, username, displayName}]`
- `RequestFill`：`{itemId}` → 返回 `{username, password}`（建议带一次性 token 与短 TTL）
- `FormSubmitted`：`{origin, username, password, isUpdateHint}`

### 3.2 安全要点
- 扩展与桌面端绑定：首次配对（随机 clientId/共享密钥）避免被其他进程伪造。
- 密码解密“尽量晚”：只在用户确认填充时才返回明文。
- 关键动作二次确认：保存/更新、复制密码、导出。

## 4. UI/交互建议
- 登录框附近的小弹窗：桌面端很难精确定位网页控件；建议由扩展在网页内渲染 UI，桌面端只负责返回候选数据与策略判断。
- 桌面端弹窗：保存/更新提示、冲突处理（同站点多账号）。
