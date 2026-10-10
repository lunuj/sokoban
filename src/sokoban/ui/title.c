/**
 * @file title.c
 * @brief 标题界面模块实现
 * @details 实现游戏标题菜单界面，包含新建游戏、选择关卡、设置和退出功能
 */

#include "title.h"

#include "scene/scene_manager.h"

#include "../sokoban.h"

#include "anim.h"

/** @brief 资源路径宏 */
#define RES(str) "res/ui/gui/" #str
/** @brief 菜单选项数量 */
#define SELECT_NUM 4

/**
 * @brief 标题界面上下文枚举
 * @details 定义标题菜单的各个选项
 */
enum TitleContext
{
    TITLE_GAME,    /**< 新建游戏 */
    TITLE_STAGE,   /**< 选择关卡 */
    TITLE_SETTING, /**< 设置 */
    TITLE_EXIT,    /**< 退出游戏 */
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
struct TitlePri
{
    Texture bg;     /**< 背景纹理 */
    int index;      /**< 当前选中的菜单项索引 */
    Texture select; /**< 选择指示器纹理 */

    Anim anim;
    Animation animation;
    Vector2 current_pos; // 插值后的位置
};

/**
 * @brief 标题界面初始化
 * @param scene 场景指针
 * @details 加载纹理资源并初始化状态
 */
static void title_init(Scene* scene)
{
    scene->pri = plugin_calloc(1, sizeof(struct TitlePri));

    struct TitlePri* pri = (struct TitlePri*)scene->pri;

    pri->bg = plugin_load_texture(RES(title.png));
    pri->index = 0;
    pri->select = plugin_load_texture(RES(select.png));

    AnimValue v = {
        .type = ANIMATION_POSITION,
        .from.position = pos[0],
        .to.position = pos[0],
    };
    animation_init(&pri->animation, 0.25f, v, &pri->current_pos);
    anim_init(&pri->anim, ANIM_ANIMATION, &pri->animation);
    pri->anim.loop = false;
}

/**
 * @brief 标题界面重置
 * @param scene 场景指针
 * @details 将选择索引重置为0
 */
static void title_reset(Scene* scene)
{
    struct TitlePri* pri = (struct TitlePri*)scene->pri;
    pri->index = 0;
}

/**
 * @brief 标题界面渲染
 * @param scene 场景指针
 * @details 绘制背景和选择指示器
 */
static void title_render(Scene* scene)
{
    struct TitlePri* pri = (struct TitlePri*)scene->pri;

    int screen_width = plugin_get_window_width();
    int screen_height = plugin_get_window_height();

    plugin_draw_texture_pro(
        pri->bg,
        (Rectangle){0, 0, pri->bg.width, pri->bg.height},
        (Rectangle){0, 0, screen_width, screen_height},
        (Vector2){0, 0},
        0,
        WHITE
    );
    plugin_draw_texture_pro(
        pri->select,
        (Rectangle){0, 0, pri->select.width, pri->select.height},
        (Rectangle){pri->current_pos.x, pri->current_pos.y, pri->select.width, pri->select.height},
        (Vector2){0, 0},
        0,
        WHITE
    );
}

/**
 * @brief 标题界面退出清理
 * @param scene 场景指针
 * @details 卸载纹理并释放内存
 */
static void title_exit(Scene* scene)
{
    struct TitlePri* pri = (struct TitlePri*)scene->pri;
    plugin_unload_texture(pri->select);
    plugin_unload_texture(pri->bg);

    plugin_free(pri);
}

/**
 * @brief 标题界面更新
 * @param scene 场景指针
 * @param c 操作码
 * @details 处理菜单导航和选项选择
 */
static void title_update(Scene* scene, int c)
{
    struct TitlePri* pri = (struct TitlePri*)scene->pri;

    anim_update(&pri->anim, 1.0 / 60);

    int* index = &pri->index;

    switch (c)
    {
    case OPTION_MOV_UP:
    case OPTION_MOV_LEFT:
        *index = (*index + SELECT_NUM - 1) % SELECT_NUM;
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = pos[*index];
        anim_reset(&pri->anim);
        break;
    case OPTION_MOV_DOWN:
    case OPTION_MOV_RIGHT:
        *index = (*index + 1) % SELECT_NUM;
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = pos[*index];
        anim_reset(&pri->anim);
        break;
    case OPTION_ENTER:
        switch (*index)
        {
        case TITLE_GAME:
            scene_manager_request(scene->mgr, scene_action_reset(GAME));
            scene_manager_request(scene->mgr, scene_action_replace(GAME));
            break;
        case TITLE_STAGE:
            scene_manager_request(scene->mgr, scene_action_reset(STAGE));
            scene_manager_request(scene->mgr, scene_action_replace(STAGE));
            break;
        case TITLE_SETTING:
            scene_manager_request(scene->mgr, scene_action_reset(SETTING));
            scene_manager_request(scene->mgr, scene_action_replace(SETTING));
            break;
        case TITLE_EXIT:
            scene_manager_request(scene->mgr, scene_action_quit());
            break;
        default:
            break;
        }
        break;
    case OPTION_BACK:
        scene->status = true;
        break;
    default:
        break;
    }
}

/** @brief 标题界面操作函数集 */
const SceneOps title_ops = {
    .init = title_init,
    .reset = title_reset,
    .render = title_render,
    .update = title_update,
    .exit = title_exit,
};