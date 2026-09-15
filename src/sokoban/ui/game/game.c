/**
 * @file game.c
 * @brief 游戏主界面模块实现
 * @details 实现游戏核心玩法，包括地图渲染、玩家控制和游戏逻辑更新
 */

#include <stdlib.h>
#include "game.h"

#include "../../sokoban.h"

#include "../../core/core.h"
#include "../../core/map.h"

#include "scene/scene_manager.h"

/** @brief 资源路径宏 */
#define RES(str) "res/img/" #str ".png"

/** @brief 地图块渲染大小（像素） */
#define MAP_BLOCK_SIZE 40

/**
 * @brief 游戏界面私有数据结构
 */
struct GamePri
{
    Map* map; /**< 地图数据 */
    Records* records;
    Texture2D map_textrue[MAX]; /**< 地图块纹理数组 */
};

/**
 * @brief 游戏界面初始化
 * @param scene 场景指针
 * @details 加载地图和纹理资源
 */
static void game_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &game_ops;
    scene->pri = calloc(1, sizeof(struct GamePri));

    struct GamePri* pri = (struct GamePri*)scene->pri;

    pri->map = sokoban_get_map();
    pri->records = sokoban_get_records();

    sokoban_load_map(sokoban_get_stage());

    pri->map_textrue[BOX] = plugin_load_texture(RES(box));
    pri->map_textrue[PLAYER] = plugin_load_texture(RES(player));
    pri->map_textrue[SPACE] = plugin_load_texture(RES(space));
    pri->map_textrue[TARGET] = plugin_load_texture(RES(target));
    pri->map_textrue[BLOCK] = plugin_load_texture(RES(block));
}

/**
 * @brief 游戏界面重置
 * @param scene 场景指针
 * @details 重新加载地图并重置录制状态s
 */
static void game_reset(Scene* scene)
{
    struct GamePri* pri = (struct GamePri*)scene->pri;
    sokoban_load_map(sokoban_get_stage());
}

/**
 * @brief 游戏界面渲染
 * @param scene 场景指针
 * @details 绘制所有地图块
 */
static void game_render(Scene* scene)
{
    struct GamePri* pri = (struct GamePri*)scene->pri;
    Map* map = pri->map;
    Texture* map_texture = pri->map_textrue;

    int w = get_map_width(map);
    int h = get_map_height(map);

    for (int i = 0; i < w * h; i++)
    {
        int x = (i % w) * MAP_BLOCK_SIZE;
        int y = (i / w) * MAP_BLOCK_SIZE;

        if (map_is(map, i, MAP_SPACE))
            plugin_draw_texture(map_texture[SPACE], x, y, WHITE);
        if (map_is(map, i, MAP_TARGET))
            plugin_draw_texture(map_texture[TARGET], x, y, WHITE);
        if (map_is(map, i, MAP_BLOCK))
            plugin_draw_texture(map_texture[BLOCK], x, y, WHITE);
        if (map_is(map, i, MAP_PLAYER))
            plugin_draw_texture(map_texture[PLAYER], x, y, WHITE);
        if (map_is(map, i, MAP_BOX))
            plugin_draw_texture(map_texture[BOX], x, y, WHITE);
    }
}

/**
 * @brief 将操作码转换为地图命令
 * @param key 操作码
 * @return 对应的地图命令
 */
static int key_to_command(int key)
{
    int ret = CMD_NONE;
    switch (key)
    {
    case OPTION_MOV_UP:
        ret = CMD_MOV_UP;
        break;
    case OPTION_MOV_DOWN:
        ret = CMD_MOV_DOWN;
        break;
    case OPTION_MOV_LEFT:
        ret = CMD_MOV_LEFT;
        break;
    case OPTION_MOV_RIGHT:
        ret = CMD_MOV_RIGHT;
        break;
    case OPTION_BACK:
        ret = CMD_BACK;
        break;
    case OPTION_ENTER:
        ret = CMD_ENTER;
        break;
    case OPTION_UNDO:
        ret = CMD_UNDO;
        break;
    default:
        break;
    }
    return ret;
}

/**
 * @brief 游戏界面更新
 * @param scene 场景指针
 * @param c 操作码
 * @details 处理玩家输入，更新游戏状态，检查胜利条件
 */
static void game_update(Scene* scene, int c)
{
    struct GamePri* pri = (struct GamePri*)scene->pri;

    switch (c)
    {
    case OPTION_BACK:
        scene_manager_reset(scene->mgr, GAME_MENU);
        scene_manager_set_overlay(scene->mgr, GAME_MENU);
        break;
    default:
        core_update(pri->map, pri->records, key_to_command(c));
        break;
    }

    if (core_is_win(pri->map))
    {
        sokoban_set_result(1);
        scene_manager_set_overlay(scene->mgr, GAME_END);
    }
}

/**
 * @brief 游戏界面退出清理
 * @param scene 场景指针
 * @details 卸载纹理并释放内存
 */
static void game_exit(Scene* scene)
{
    struct GamePri* pri = (struct GamePri*)scene->pri;
    for (int i = 0; i < MAX; i++)
    {
        plugin_unload_texture(pri->map_textrue[i]);
    }

    free(pri);
}

/** @brief 游戏界面操作函数集 */
const SceneOps game_ops = {
    .init = game_init,
    .reset = game_reset,
    .render = game_render,
    .update = game_update,
    .exit = game_exit
};