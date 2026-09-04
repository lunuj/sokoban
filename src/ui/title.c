/**
 * @file title.c
 * @brief 标题界面模块实现
 * @details 实现游戏标题菜单界面，包含新建游戏、选择关卡、设置和退出功能
 */

#include <stdlib.h>

#include "title.h"

#include "raylib/raylib.h"
#include "scene/scene.h"

/** @brief 资源路径宏 */
#define RES(str) "res/ui/gui/" #str
/** @brief 菜单选项数量 */
#define SELECT_NUM 4

/**
 * @brief 标题界面上下文枚举
 * @details 定义标题菜单的各个选项
 */
enum TitleContext {
    NEW,     /**< 新建游戏 */
    SELECT,  /**< 选择关卡 */
    SETTING, /**< 设置 */
    EXIT,    /**< 退出游戏 */
};

/** @brief 菜单选项位置坐标数组 */
static Vector2 pos[SELECT_NUM] = {
    {200, 150}, /**< 新建游戏位置 */
    {200, 190}, /**< 选择关卡位置 */
    {200, 240}, /**< 设置位置 */
    {200, 290}, /**< 退出位置 */
};

/**
 * @brief 标题界面私有数据结构
 */
struct TitlePri {
    Texture2D bg;     /**< 背景纹理 */
    int index;        /**< 当前选中的菜单项索引 */
    Texture2D select; /**< 选择指示器纹理 */
};

/**
 * @brief 标题界面初始化
 * @param scene 场景指针
 * @details 加载纹理资源并初始化状态
 */
static void title_init(Scene *scene)
{
    scene->end = false;
    scene->ops = title_get_ops();
    scene->pri = calloc(1, sizeof(struct TitlePri));

    struct TitlePri *pri = (struct TitlePri *)scene->pri;

    pri->bg = LoadTexture(RES(title.png));
    pri->index = 0;
    pri->select = LoadTexture(RES(select.png));
}

/**
 * @brief 标题界面重置
 * @param scene 场景指针
 * @details 将选择索引重置为0
 */
static void title_reset(Scene *scene)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;
    pri->index = 0;
}

/**
 * @brief 标题界面渲染
 * @param scene 场景指针
 * @details 绘制背景和选择指示器
 */
static void title_render(Scene *scene)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    ClearBackground(WHITE);
    DrawTexturePro(pri->bg, (Rectangle){0, 0, pri->bg.width, pri->bg.height},
                   (Rectangle){0, 0, screen_width, screen_height},
                   (Vector2){0, 0}, 0, WHITE);
    DrawTexturePro(pri->select,
                   (Rectangle){0, 0, pri->select.width, pri->select.height},
                   (Rectangle){pos[pri->index].x, pos[pri->index].y,
                               pri->select.width, pri->select.height},
                   (Vector2){0, 0}, 0, WHITE);
}

/**
 * @brief 标题界面退出清理
 * @param scene 场景指针
 * @details 卸载纹理并释放内存
 */
static void title_exit(Scene *scene)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;
    UnloadTexture(pri->select);
    UnloadTexture(pri->bg);

    free(pri);
}

/**
 * @brief 标题界面更新
 * @param scene 场景指针
 * @param c 操作码
 * @details 处理菜单导航和选项选择
 */
static void title_update(Scene *scene, int c)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;

    int *index = &pri->index;
    switch (c) {
    case OPTION_MOV_UP:
    case OPTION_MOV_LEFT:
        *index = (*index + SELECT_NUM - 1) % SELECT_NUM;
        break;
    case OPTION_MOV_DOWN:
    case OPTION_MOV_RIGHT:
        *index = (*index + 1) % SELECT_NUM;
        break;
    case OPTION_ENTER:
        switch (*index) {
        case NEW:
            scene_reset(GAME);
            scene_set_current(GAME);
            break;
        case SELECT:
            break;
        case SETTING:
            break;
        case EXIT:
            scene_set_status(TITLE, true);
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

/** @brief 标题界面操作函数集 */
static SceneOps title_ops = {
    .init = title_init,
    .reset = title_reset,
    .render = title_render,
    .update = title_update,
    .exit = title_exit,
};

/**
 * @brief 获取标题界面操作函数集
 * @return SceneOps结构体
 */
SceneOps title_get_ops()
{
    return title_ops;
}