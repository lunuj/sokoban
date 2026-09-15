#include "sokoban.h"

#include "plugin/plugin_api.h"

#include "scene/scene_manager.h"
#include "version.h"

#include "ui/game/end.h"
#include "ui/game/game.h"
#include "ui/game/menu.h"
#include "ui/setting.h"
#include "ui/stage.h"
#include "ui/title.h"

#include "core/map.h"
#include "core/records.h"
#include "tool/xsb.h"

/**
 * @brief 按键映射函数
 * @param c 原始按键值
 * @return 映射后的操作码
 * @details 将键盘按键映射为统一的选项操作码
 */
int sokoban_key_map(int c)
{
    int ret = OPTION_NONE;
    switch (c)
    {
    case KEY_W:
    case KEY_UP:
        ret = OPTION_MOV_UP;
        break;
    case KEY_S:
    case KEY_DOWN:
        ret = OPTION_MOV_DOWN;
        break;
    case KEY_A:
    case KEY_LEFT:
        ret = OPTION_MOV_LEFT;
        break;
    case KEY_D:
    case KEY_RIGHT:
        ret = OPTION_MOV_RIGHT;
        break;
    case KEY_ESCAPE:
        ret = OPTION_BACK;
        break;
    case KEY_ENTER:
        ret = OPTION_ENTER;
        break;
    case KEY_E:
        ret = OPTION_UNDO;
        break;
    case KEY_R:
        ret = OPTION_RELOAD;
    default:
        break;
    }
    return ret;
}

/**
 * @brief 插件私有数据（场景管理器）
 * @details 保存所有场景实例和当前场景索引
 */
struct Sokoban
{
    uint32_t magic;
    Version version;

    int current_stage;
    bool is_win;
    Map map;
    Records records;

    SceneManager* mgr;
    PlatformApi* api;
};

static struct Sokoban* pri;
static const SceneOps* scene_ops[SCENE_MAX] = {
    &title_ops,
    &stage_ops,
    &setting_ops,
    &game_ops,
    &menu_ops,
    &end_ops,
};

void sokoban_set_result(bool result)
{
    pri->is_win = result;
}

bool sokoban_get_result()
{
    return pri->is_win;
}

void sokoban_set_stage(int stage)
{
    pri->current_stage = stage;
}

int sokoban_get_stage()
{
    return pri->current_stage;
}

Map* sokoban_get_map(void)
{
    return &pri->map;
}

void sokoban_load_map(int stage)
{
    char str[64];
    snprintf(str, sizeof(str), "res/map/map%02d.xsb", stage);
    load_map_from_xsb(str, &pri->map);
}

Records* sokoban_get_records(void)
{
    return &pri->records;
}

void sokoban_init(PlatformApi* api)
{
    plugin_set_api(api);

    pri = plugin_calloc(1, sizeof(struct Sokoban));
    if (pri == NULL)
    {
        exit(0);
    }

    pri->magic = 0xdeadbeef;
    pri->version = VERSION;
    pri->api = api;

    pri->current_stage = 1;

    pri->mgr = scene_manager_create(SCENE_MAX);
    for (int i = 0; i < SCENE_MAX; i++)
    {
        scene_manager_register(pri->mgr, i, scene_ops[i]);
    }

    scene_manager_push(pri->mgr, TITLE);
}

void sokoban_exit(void)
{
    scene_manager_destroy(pri->mgr);
    plugin_free(pri);
}

void sokoban_render(void)
{
    plugin_clear_background(BLANK);
    plugin_begin_drawing();
    scene_manager_render(pri->mgr);
    plugin_end_drawing();
}

void sokoban_update(int key)
{
    int mapped = sokoban_key_map(key);
    if (scene_manager_update(pri->mgr, mapped))
    {
        plugin_set_window_close();
    }
}

void* sokoban_pre_reload()
{
    return pri;
}

bool sokoban_post_reload(void* payload)
{
    bool ret = false;
    struct Sokoban* ptr = (struct Sokoban*)payload;
    Version version = VERSION;

    if (version_is_compatible(&version, &ptr->version))
    {
        pri = ptr;
        plugin_set_api(pri->api);
        for (int i = 0; i < SCENE_MAX; i++)
        {
            scene_manager_reload(pri->mgr, i, scene_ops[i]);
        }
        ret = true;
        plugin_log(0, "reload ok\n");
    }

    return ret;
}

PluginApi PLUGIN_API_NAME = {
    .name = "sokoban",
    .version = VERSION,
    .init = sokoban_init,
    .exit = sokoban_exit,
    .render = sokoban_render,
    .update = sokoban_update,
    .pre_reload = sokoban_pre_reload,
    .post_reload = sokoban_post_reload
};