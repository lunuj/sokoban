/**
 * @file end.c
 * @brief 游戏结束界面模块实现
 * @details 显示游戏胜利或失败的结果界面
 */

#include "end.h"

#include "../../sokoban.h"

#include "scene/scene_manager.h"

/** @brief 结束界面选项数量 */
#define MAX_NUM 2

/** @brief 结束界面文本数组 */
char* end_text[MAX_NUM] = {
    "You Win !!",  /**< 胜利文本 */
    "You Lose !!", /**< 失败文本 */
};

/**
 * @brief 结束界面私有数据结构
 */
struct EndPri
{
    int index;   /**< 当前显示的结果索引（0=胜利，1=失败） */
    char** text; /**< 结束文本数组 */
};

/**
 * @brief 结束界面初始化
 * @param scene 场景指针
 * @details 初始化结束界面状态
 */
static void end_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &end_ops;
    scene->pri = plugin_calloc(1, sizeof(struct EndPri));

    struct EndPri* pri = (struct EndPri*)scene->pri;
    pri->index = 0;
    pri->text = end_text;
}

/**
 * @brief 结束界面重置
 * @param scene 场景指针
 * @details 将结果索引重置为0（胜利）
 */
static void end_reset(Scene* scene)
{
    struct EndPri* pri = (struct EndPri*)scene->pri;

    pri->index = 0;
}

/**
 * @brief 结束界面渲染
 * @param scene 场景指针
 * @details 绘制半透明背景和结果文本
 */
static void end_render(Scene* scene)
{
    struct EndPri* pri = (struct EndPri*)scene->pri;

    int screen_width = plugin_get_window_width();
    int screen_height = plugin_get_window_height();

    plugin_draw_rectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 180});
    plugin_draw_text(
        pri->text[pri->index],
        screen_width / 2,
        screen_height / 2,
        64,
        pri->index == 1 ? YELLOW : RED
    );
}

/**
 * @brief 结束界面更新
 * @param scene 场景指针
 * @param c 操作码
 * @details 根据操作码显示胜利或失败，任意按键返回标题界面
 */
static void end_update(Scene* scene, int c)
{
    struct EndPri* pri = (struct EndPri*)scene->pri;
    pri->index = sokoban_get_result();

    switch (c)
    {
    case OPTION_NONE:
        break;
    default:
        scene_manager_replace(scene->mgr, TITLE);
        break;
    }
}

/**
 * @brief 结束界面退出清理
 * @param scene 场景指针
 * @details 无资源需要释放
 */
static void end_exit(Scene* scene)
{
    (void)scene;
}

/** @brief 结束界面操作函数集 */
const SceneOps end_ops = {
    .init = end_init,
    .reset = end_reset,
    .render = end_render,
    .update = end_update,
    .exit = end_exit,
};
