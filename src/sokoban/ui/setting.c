#include "setting.h"

#include <stdio.h>

#include "../sokoban.h"
#include "scene/scene_manager.h"

#define MAX_NUM 4

char* setting_text[MAX_NUM] = {
    "render setting", /**< 胜利文本 */
    "audio setting",  /**< 失败文本 */
    "game setting",   /**< 失败文本 */
    "back to title",  /**< 失败文本 */
};

struct SettingPri
{
    int index;
    char** text; /**< 结束文本数组 */
};

static void setting_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &setting_ops;
    scene->pri = plugin_calloc(1, sizeof(struct SettingPri));

    struct SettingPri* pri = (struct SettingPri*)scene->pri;

    pri->index = 0;
    pri->text = setting_text;
}

static void setting_reset(Scene* scene)
{
    struct SettingPri* pri = (struct SettingPri*)scene->pri;

    pri->index = 0;
}

static void setting_render(Scene* scene)
{
    struct SettingPri* pri = (struct SettingPri*)scene->pri;
    char tmp[32];

    int screen_width = plugin_get_window_width();
    int screen_height = plugin_get_window_height();

    for (int i = 0; i < MAX_NUM; i++)
    {
        plugin_draw_text(pri->text[i], 0, 24 * i, 24, pri->index == i ? YELLOW : RED);
    }
}

static void setting_update(Scene* scene, int key)
{
    struct SettingPri* pri = (struct SettingPri*)scene->pri;
    int index = pri->index;

    switch (key)
    {
    case OPTION_MOV_UP:
    case OPTION_MOV_LEFT:
        index = (index - 1 + MAX_NUM) % MAX_NUM;
        break;
    case OPTION_MOV_DOWN:
    case OPTION_MOV_RIGHT:
        index = (index + 1) % MAX_NUM;
        break;
    case OPTION_ENTER:
        scene->status = true;
        break;
    case OPTION_BACK:
        scene_manager_replace(scene->mgr, TITLE);
        scene->status = true;
        break;
    default:
        break;
    }

    pri->index = index;
}

static void setting_exit(Scene* scene)
{
    plugin_free(scene->pri);
}

const SceneOps setting_ops = {
    .init = setting_init,
    .reset = setting_reset,
    .render = setting_render,
    .update = setting_update,
    .exit = setting_exit
};