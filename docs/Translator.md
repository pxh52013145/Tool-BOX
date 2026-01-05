# Translator 方案细化（草案）

> 当前实现已拆分为独立程序 `ToolboxTranslate`，可由 `ToolboxLauncher` 启动；退出启动器不影响翻译工具运行。

## 1. MVP 功能清单
- 全局热键触发翻译（可配置）。
- 取词：UIA 选中文本优先，剪贴板兜底（读取后还原剪贴板）。
- 浮窗展示：就近弹出、可固定、自动消失；支持复制译文。
- Provider：先接一个可用的 HTTP 翻译接口（用户自填 key），并提供 `MockProvider` 方便本地调试。
- 缓存：避免重复请求。

## 2. 取词实现建议（Windows）
系统级划词翻译的难点在于“如何获取选中的文本”。在浏览器外（如 Word、PDF 阅读器中），操作系统并没有统一的“获取当前选中文本”的接口，因此必须多路兜底。

推荐实现顺序（从稳定到兜底）：
1) UI Automation：通过前台窗口句柄定位 `IUIAutomationElement`，尝试 `TextPattern/Selection`。
2) 剪贴板兜底：模拟复制快捷键（默认 `Ctrl+C`），读取剪贴板文本，并尽量还原剪贴板内容（MVP 先支持纯文本）。
3) OCR（后续）：截图选区 + OCR 取词（适合图片/不可选中的文字）。

## 3. 浮窗与悬浮球
- 浮窗：无边框、置顶、阴影、圆角；不抢焦点（Windows 需要无激活窗口能力，Qt 侧走平台适配）。
- 悬浮球：可拖拽、贴边、透明度；悬停/点击弹出翻译面板。
- 黑名单：允许用户对某些应用禁用（游戏/全屏/敏感软件）。

## 4. Provider 抽象（建议接口）
- `translate(text, sourceLang, targetLang, context)` → `{detectedLang, translatedText, alternatives?}`
- 需要支持：超时/重试/限流、错误分级（网络/鉴权/额度/参数）、结果缓存（同文本+语言对 LRU+TTL）。

## 5. 资料与对标（你收集的参考，保留）
### 5.1 最佳 Qt 参考：Crow Translate (C++ / Qt)
这是目前 Qt 生态中最好的开源翻译工具，完全符合你的需求。
- 项目地址：`https://github.com/crow-translate/crow-translate`
- 值得参考的核心技术点：
  - 屏幕取词逻辑：监听鼠标/键盘快捷键 -> 模拟发送 `Ctrl+C` -> 读取剪贴板 -> 恢复剪贴板。
  - OCR（屏幕截图翻译）：集成 Tesseract OCR。
  - API 集成：封装 Google、DeepL、Bing 等翻译引擎的请求。
  - 全局快捷键：使用 `QHotkey` 或平台原生 API 注册。

### 5.2 最佳交互参考：Pot (Rust + Tauri)
虽然不是 Qt 开发，但它是目前体验很好的“划词翻译”桌面工具之一。
- 项目地址：`https://github.com/pot-app/pot-desktop`
- 值得参考的设计点：
  - UI 交互：“悬停小球”和“划词后即时图标”的交互逻辑。
  - 插件化接口：可扩展不同翻译源插件。

## 6. 代码位置（当前实现）
- 程序入口：`apps/translate_app/*`
- 数据库：`src/translate/translatedatabase.*`（独立 SQLite 文件：`translate.sqlite3`）
- Provider/Service：`src/translate/*`
- 历史记录 Model/View：`src/translate/translationhistorymodel.*`、`src/pages/translatorpage.*`
