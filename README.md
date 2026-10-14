# Handmake — 推箱子游戏

一个使用 **纯 C** 编写的推箱子（Sokoban）游戏，采用**插件化 + 场景管理 + 平台抽象**的模块化架构，基于 [raylib](https://www.raylib.com/) 渲染。

---

## ✨ 特性

- 🎮 **完整玩法**：移动、撤销 / 重做、录像、关卡选择、统计信息
- ✏️ **关卡编辑器**：内置可视化编辑器，支持保存为 XSB 格式
- 💾 **存档系统**：基于 `sokoban.archive` 持久化游戏进度
- 🎬 **动画系统**：自研通用动画框架，支持序列 / 并行 / 循环
- 🔌 **插件热加载**：基于 `dlopen` 的动态库机制，运行时按 `R` 重新加载 `.so`
- 🗺️ **大地图支持**：玩家视角跟随移动，自动滚屏到边界
- 🌐 **跨平台构建**：一次构建，Linux / Windows / macOS 通吃
- 🔢 **自动版本号**：根据 Git 信息自动生成 SemVer 版本字符串

---

## 🏗️ 架构概览

```
┌──────────────────────────────────────────────┐
│  Host (main/plugin/scene.c)                  │
│   ↓ plugin_init                              │
│  Plugin Manager (src/plugin/)                │
│   └─ dlopen("libplug.so")                    │
│      ↓                                        │
│      ┌──────────────────────────────┐        │
│      │  Plugin (.so)                │        │
│      │  ├─ Scene Manager            │        │
│      │  ├─ Sokoban Game Logic       │        │
│      │  ├─ Animation System         │        │
│      │  └─ UI (title/stage/game/…)  │        │
│      └──────────────────────────────┘        │
│         ↓ PlatformApi (function table)      │
│  Platform Abstraction (src/platform/)       │
│   └─ raylib (libraylib.so / .a / .lib)     │
└──────────────────────────────────────────────┘
```

**核心设计要点**：

| 层级 | 模块 | 职责 |
|------|------|------|
| 平台抽象 | `src/platform/` | 封装 raylib，向插件提供 `PlatformApi` 函数表 |
| 插件管理 | `src/plugin/` | 动态库加载、版本检查、热重载、生命周期 |
| 场景管理 | `src/scene/` | 场景栈（push/pop/replace/overlay）、延迟请求队列 |
| 动画系统 | `src/anim.{c,h}` | 通用动画原语（Sequence / Parallel / Loop） |
| 游戏逻辑 | `src/sokoban/` | 地图、关卡、记录、存档、XSB 格式 |
| UI 场景 | `src/sokoban/ui/` | 标题 / 关卡选择 / 设置 / 编辑器 / 游戏 / 菜单 / 结束 |

---

## 📂 目录结构

```
handmake/
├── build.sh                # 构建脚本（封装 make）
├── Makefile                # 自动检测 OS + 注入 Git 版本
├── docs/                   # 设计文档（架构.md / plan.md / next.md / todo.md）
├── include/raylib/         # raylib 头文件（vendored）
├── lib/
│   ├── Linux/              # libraylib.a / .so / .so.600
│   └── Windows/raylib/     # Windows 预编译库
├── main/                   # 宿主程序入口
│   ├── core/{cli,map}.c   # 命令行 / 地图工具
│   ├── ui/gui.c            # GUI 主程序（静态构建）
│   └── plugin/scene.c      # 插件模式主程序（默认入口）
├── modules/                # 预留扩展子模块
└── src/                    # 核心源码
    ├── platform/           # 平台抽象层
    ├── plugin/             # 插件系统
    ├── scene/              # 场景管理
    ├── sokoban/            # 推箱子游戏
    │   ├── core/           #   地图、记录
    │   ├── tool/           #   XSB 格式读写
    │   └── ui/             #   各场景实现
    └── anim.{c,h}          # 通用动画系统
```

---

## 🚀 构建与运行

### 环境要求

- **编译器**：GCC / Clang（支持 C11）
- **构建工具**：GNU Make、Bash
- **依赖**：raylib（已预编译在 `lib/{OS}/` 中）

### 常用命令

```bash
# 显示帮助
./build.sh help

# 编译并运行插件模式主程序
./build.sh run main

# 仅编译指定 target
./build.sh build main

# 生成动态库（libplug.so）
./build.sh dynamic

# 生成静态库（libplug.a）
./build.sh static

# 清理输出
./build.sh clean
```

### 编译产物

- 可执行文件：`output/main.out`
- 动态库：`output/libplug.so`
- 静态库：`output/libplug.a`
- 依赖文件：`output/.dep/` 与 `output/.obj/`

---

## 🎮 操作键位

| 按键 | 操作 |
|---|---|
| `W` / `↑` | 向上移动 |
| `S` / `↓` | 向下移动 |
| `A` / `←` | 向左移动 |
| `D` / `→` | 向右移动 |
| `E` | 撤销 |
| `F` | 重做 |
| `R` | 重新加载插件（热重载） |
| `Enter` | 确认 |
| `Esc` | 返回 / 退出 |
| `1`~`7` | 编辑器笔刷切换 |

---

## 🌐 平台支持

`Makefile` 会根据 `uname -s` 自动选择系统依赖：

| 平台 | 链接库 |
|------|--------|
| Linux | `-ldl -lpthread -lm -lX11` |
| macOS | Cocoa / AppKit / Foundation / OpenGL / … |
| Windows | gdi32 / user32 / kernel32 / opengl32 / winmm |

也可通过环境变量强制指定：
```bash
make OS=Windows
make OS=Mac
```

---

## 🔢 版本管理

`make` 会在编译时自动生成 `src/version_auto.c`，根据 Git 状态输出：

| 仓库状态 | 版本字符串示例 |
|---------|--------------|
| **干净（已提交）** | `0.1.2-release + a1b2c3d-42` |
| **脏（有未提交改动）** | `0.1.2-debug + user-2026.10.14-15:30:42` |

可通过 `make echo-version` 查看当前版本信息。

---

## 🧩 扩展方式

### 添加新场景
1. 在 `src/sokoban/ui/` 新建场景目录（含 `init / reset / update / render / exit`）
2. 在 `sokoban.h` 的 `SceneType` 枚举中追加
3. 在 `sokoban.c` 的 `sokoban_init` 中 `scene_manager_register`

### 编写新插件
1. 包含 `plugin/plugin_api.h`
2. 实现 `PluginApi` 结构体（含 `name / version / init / exit / pre_reload / post_reload / update / render`）
3. 用 `PLUGIN_API_NAME` 符号导出
4. 链接 `libplug.so` 提供的 `PlatformApi`，无需依赖 raylib

---

## 📜 项目演进

按 `docs/todo.md` 记录，已完成：

- [x] 关卡系统
- [x] Scene 通用化（Resource + Context 抽象）
- [x] 录像优化
- [x] 大地图与视角移动
- [x] 自动版本号
- [x] XSB 变长行宽度解析
- [x] 视角移动优化（边界局部滚动）
- [x] 统计信息与 UI
- [x] 动画系统
- [x] 存档系统
- [x] 关卡编辑器

---

## 📄 License

MIT License

---

## 🙏 致谢

- [raylib](https://www.raylib.com/) — 简洁易用的 C 语言游戏框架
- 所有贡献者