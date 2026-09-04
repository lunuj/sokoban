/**
 * @file menu.c
 * @brief 游戏菜单模块实现
 * @details 实现游戏内暂停菜单，包含继续、重置、返回主页和退出游戏功能
 */

#include <stdlib.h>
#include "menu.h"

#include "raylib/raylib.h"
#include "scene/scene.h"

/** @brief 菜单选项数量 */
#define MAX_NUM 4

/**
 * @brief 菜单上下文枚举
 * @details 定义菜单的各个选项
 */
enum MenuContext
{
    CONTINUE,       /**< 继续游戏 */
    RESUME,         /**< 重置游戏 */
    BACK_TO_HOME,   /**< 返回主页 */
    EXIT,           /**< 退出游戏 */
};

/** @brief 菜单文本数组 */
char *menu_text[MAX_NUM] = {
    "continue game",
    "restore game",
    "back to home",
    "exit game",
};

/**
 * @brief 菜单私有数据结构
 */
struct MenuPri
{
    int index;      /**< 当前选中的菜单项索引 */
    char **text;    /**< 菜单文本数组 */
};

/**
 * @brief 菜单界面初始化
 * @param scene 场景指针
 * @details 初始化菜单状态
 */
static void menu_init(Scene *scene)
{
    scene->end = false;
    scene->ops = menu_get_ops();
    scene->pri = calloc(1, sizeof(struct MenuPri));

    struct MenuPri *pri = (struct MenuPri *)scene->pri;
    pri->index = 0;
    pri->text = menu_text;
}

/**
 * @brief 菜单界面重置
 * @param scene 场景指针
 * @details 将选中索引重置为0
 */
static void menu_reset(Scene *scene)
{
    struct MenuPri *pri = (struct MenuPri *)scene->pri;

    pri->index = 0;
}

/**
 * @brief 菜单界面渲染
 * @param scene 场景指针
 * @details 绘制半透明背景和菜单选项
 */
static void menu_render(Scene *scene)
{
    struct MenuPri *pri = (struct MenuPri *)scene->pri;

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    DrawRectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 180});
    for (int i = 0; i < MAX_NUM; i++)
    {
        if (i == pri->index)
            DrawText(pri->text[i], 0, i * 20, 14, YELLOW);
        else
            DrawText(pri->text[i], 0, i * 20, 14, RED);
    }
}

/**
 * @brief 菜单界面更新
 * @param scene 场景指针
 * @param c 操作码
 * @details 处理菜单导航和选项选择
 */
static void menu_update(Scene *scene, int c)
{
    struct MenuPri *pri = (struct MenuPri *)scene->pri;

    int *index = &pri->index;
    switch (c)
    {
    case OPTION_MOV_UP:
    case OPTION_MOV_LEFT:
        *index = (*index + MAX_NUM - 1) % MAX_NUM;
        break;
    case OPTION_MOV_DOWN:
    case OPTION_MOV_RIGHT:
        *index = (*index + 1) % MAX_NUM;
        break;
    case OPTION_ENTER:
        switch (*index)
        {
        case CONTINUE:
            scene->end = true;
            break;
        case RESUME:
            scene_reset(GAME);
            scene->end = true;
            break;
        case BACK_TO_HOME:
            scene_reset(GAME_END);
            scene_set_overlay(GAME, GAME_END);
            scene_update(OPTION_LOSE);
            scene->end = true;
            break;
        case EXIT:
            scene_set_current(TITLE);
            scene_set_status(TITLE, true);
            scene->end = true;
            break;
        default:
            break;
        }
        break;
    case OPTION_BACK:
        scene->end = true;
        break;
    default:
        break;
    }
}

/**
 * @brief 菜单界面退出清理
 * @param scene 场景指针
 * @details 无资源需要释放
 */
static void menu_exit(Scene *scene) {
    (void)scene;
}

/** @brief 菜单界面操作函数集 */
static SceneOps menu = {
    .init = menu_init,
    .reset = menu_reset,
    .render = menu_render,
    .update = menu_update,
    .exit = menu_exit,
};

/**
 * @brief 获取菜单界面操作函数集
 * @return SceneOps结构体
 */
SceneOps menu_get_ops()
{
    return menu;
}