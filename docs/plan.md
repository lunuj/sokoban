推箱子游戏项目
---
基于现有的代码结构和功能，我们可以从**玩法体验、视觉交互、工程架构、内容扩展**四个维度来探讨改进方向。

## 一、玩法与体验改进

### 1.1 关卡系统
**现状**：只硬编码加载 `map01.txt`，没有关卡选择。

**改进方案**：
```c
// 增加关卡管理
typedef struct {
    int currentLevel;
    int totalLevels;
    int unlockedLevels;
} LevelManager;

// 支持多关卡加载
void load_level(Map *map, int level_index) {
    char path[64];
    snprintf(path, sizeof(path), "res/map/map%02d.txt", level_index);
    load_map_from_txt(path, map);
}
```

### 1.2 步数统计与计时器
```c
typedef struct {
    int moves;          // 移动步数
    int pushes;         // 推箱次数
    float elapsedTime;  // 通关用时
} GameStats;

// 在 mov() 中统计
void mov(Map *map, int pos_f, int direct, int step) {
    // ... 原有逻辑 ...
    if (map_is(map, pos_t, MAP_BOX)) {
        game_stats.pushes++;
    }
    game_stats.moves++;
}
```

### 1.3 提示/预览系统
- **死局检测**：检测箱子是否被推到角落（除非那是目标点）
- **预览功能**：按空格键显示下一步最佳移动路径
- **重置当前关卡**：`R` 键重置，而非只能重启游戏

### 1.4 多步骤撤销增强
**现状**：只支持单步撤销。
**改进**：支持撤销历史浏览，或一键撤销到某一步。

---

## 二、视觉与交互改进

### 2.1 动画流畅度
**现状**：`plug.c` 中有动画系统，但未与游戏逻辑集成。

**改进**：将动画系统应用到游戏移动中
```c
// 移动动画
typedef struct {
    int from_pos;
    int to_pos;
    float progress;
    Animation move_anim;
} MoveAnimation;

// 在 game_update 中触发动画而非立即移动
void game_update(Scene *scene, int c) {
    if (!is_animating) {
        // 计算目标位置，启动动画
        start_move_animation(&anim, from, to);
    } else {
        // 更新动画进度
        update_move_animation(&anim, dt);
    }
}
```

### 2.2 画面视觉效果
- **背景与主题**：支持多套主题切换（暗色、古典、像素风）
- **粒子效果**：箱子推到目标点时播放粒子特效
- **平滑相机**：相机跟随玩家移动，使用 `Camera2D` 的平滑插值

```c
// 平滑跟随
Vector2 target_pos = {player.x * BLOCK_SIZE, player.y * BLOCK_SIZE};
camera.target = Vector2Lerp(camera.target, target_pos, 0.1f);
```

### 2.3 UI 增强
- **HUD 显示**：步数、推箱数、用时、当前关卡
- **进度条**：显示当前关卡完成度（已到位箱子/总箱子数）
- **成就系统**：如"少于10步通关"、"连续推5个箱子"

---

## 三、工程架构改进

### 3.1 资源管理
**现状**：资源散落在各场景的 `init` 函数中加载。

**改进**：统一资源管理器
```c
typedef struct {
    Texture2D textures[TEXTURE_MAX];
    Font fonts[FONT_MAX];
    Sound sounds[SOUND_MAX];
} ResourceManager;

void resources_load_all(void);
void resources_unload_all(void);
Texture2D* resources_get(ResourceType type, int index);
```

### 3.2 配置系统
```c
typedef struct {
    int default_fps;
    bool enable_animation;
    bool enable_sound;
    float master_volume;
    char language[8];
} GameConfig;

// 从 .ini 或 .json 加载
GameConfig config_load(const char *path);
void config_save(const char *path, GameConfig *config);
```

### 3.3 事件系统
替代当前的直接函数调用，使用发布-订阅模式：
```c
typedef struct {
    int type;  // EVENT_MOVE, EVENT_PUSH, EVENT_WIN
    void *data;
} Event;

void event_emit(Event *e);
void event_register(int type, void (*callback)(Event*));
```

### 3.4 日志系统
```c
typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;
void log_write(LogLevel level, const char *fmt, ...);
// 支持输出到文件和控制台
```

### 3.5 热重载稳定性（重要！）
**现状**：全局变量在重载时可能地址失效。

**改进**：所有状态封装到 `Plug` 结构体
```c
typedef struct {
    size_t size;
    // 原来所有全局变量移到这里
    Font font;
    Camera2D camera;
    Animation animations[10];
    Vector2 points[4];
    Color colors[3];
    Anim anim_nodes[16];
    Sequence seq;
    Parallel para;
    // ...
} PlugState;

static PlugState *state = NULL;

void *plug_pre_reload(void) {
    return state;  // 返回完整状态
}

void plug_post_reload(void *ptr) {
    state = (PlugState*)ptr;
    // 不需要重新分配，直接使用
    // 只需重新加载 GPU 资源（纹理、字体）
    load_resources();
}
```

---

## 四、内容与扩展

### 4.1 关卡编辑器
- 在游戏中集成编辑模式
- 支持鼠标点击放置/删除元素
- 导出为 `.txt` 或 `.bin` 格式

```c
typedef enum {
    EDITOR_MODE_VIEW,
    EDITOR_MODE_PLACE,
    EDITOR_MODE_DELETE,
} EditorMode;

void editor_update(Scene *scene, MouseState mouse);
void editor_render(Scene *scene);
```

### 4.2 关卡包支持
- 支持从外部文件加载关卡包
- 社区关卡分享机制
- 关卡难度评级

### 4.3 游戏模式扩展
- **限时模式**：规定时间内完成
- **最少步数模式**：超过步数则失败
- **无尽模式**：随机生成关卡
- **双人模式**：合作或竞技

### 4.4 存档系统
```c
typedef struct {
    int level_progress[100];  // 每个关卡的最高星数/步数
    int total_time;
    int total_moves;
    time_t last_played;
} SaveData;

void save_game(const char *path, SaveData *data);
SaveData load_game(const char *path);
```

### 4.5 本地化（多语言）
```c
typedef struct {
    char *title_new_game;
    char *title_continue;
    char *game_moves;
    char *game_pushes;
    // ...
} Localization;

Localization l10n_load(const char *lang);
```

---

## 五、性能优化

### 5.1 渲染批处理
**现状**：每个格子独立调用 `DrawTexture`。
**改进**：使用 `rlRenderBatch` 或 raylib 的纹理图集（Texture Atlas），减少 Draw Call。

### 5.2 地图渲染优化
只渲染屏幕可见区域：
```c
// 计算可见范围
int start_x = camera.target.x / BLOCK_SIZE - 2;
int end_x = start_x + screen_width / BLOCK_SIZE + 4;
// 只绘制可见区域内的格子
```

### 5.3 内存优化
- 使用位域存储地图数据（当前 `char` 只用了低4位）
- 动态分配地图大小，而非固定 `MAP_MAX_SIZE`

---

## 六、优先级建议

| 优先级 | 改进项 | 难度 | 价值 |
|--------|--------|------|------|
| 🔴 高 | 热重载稳定性修复 | 中 | 极高 |
| 🔴 高 | 关卡系统（多关卡） | 低 | 高 |
| 🔴 高 | 步数统计与UI | 低 | 高 |
| 🟡 中 | 动画集成到游戏 | 中 | 高 |
| 🟡 中 | 资源管理器 | 低 | 中 |
| 🟡 中 | 存档系统 | 中 | 高 |
| 🟢 低 | 关卡编辑器 | 高 | 中 |
| 🟢 低 | 多语言支持 | 低 | 低 |

---

## 七、具体代码示例：关卡切换

```c
// game.c 中添加
static int current_level = 1;
static int total_levels = 10;

void load_next_level(Scene *scene) {
    struct GamePri *pri = (struct GamePri *)scene->pri;
    current_level++;
    if (current_level > total_levels) {
        // 所有关卡完成
        scene_set_overlay(GAME, GAME_END);
        scene_update(OPTION_WIN);
        return;
    }
    load_map(&pri->map, current_level);
    pri->map.is_recording = true;
    // 重置统计
    game_stats.moves = 0;
    game_stats.pushes = 0;
}

void game_update(Scene *scene, int c) {
    // ... 
    if (is_win(&pri->map)) {
        // 延迟进入下一关（显示庆祝动画）
        load_next_level(scene);
    }
}
```

---

## 总结

这个项目已经具备了良好的基础架构，下一步的改进重点应该是：

1. **完善游戏性**（关卡、统计、动画）
2. **增强稳定性**（热重载、资源管理）
3. **提升用户体验**（UI、特效、音效反馈）

建议按照"先稳定后功能"的原则，优先修复热重载的内存问题，然后逐步添加关卡系统和视觉优化。