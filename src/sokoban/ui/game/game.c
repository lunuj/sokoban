/**
 * @file game.c
 * @brief 游戏主界面模块实现
 * @details 实现游戏核心玩法，包括地图渲染、玩家控制和游戏逻辑更新
 */

#include "game.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../core/core.h"
#include "../../core/map.h"
#include "../../sokoban.h"
#include "scene/scene_manager.h"

#include "anim.h"

#define DEADZONE_W 160
#define DEADZONE_H 120

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
    Camera2D camera;
    Texture2D map_textrue[MAX]; /**< 地图块纹理数组 */

    int index;
    Anim anim;
    Animation animation;
    Vector2 current_pos; // 插值后的位置
};

static Vector2 game_pos_to_position(Map* map, int pos)
{
    int w = get_map_width(map);
    int h = get_map_height(map);

    int pos_x = pos % w * MAP_BLOCK_SIZE;
    int pos_y = pos / w * MAP_BLOCK_SIZE;

    return (Vector2){pos_x, pos_y};
}

/**
 * @brief 游戏界面初始化
 * @param scene 场景指针
 * @details 加载地图和纹理资源
 */
static void game_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &game_ops;
    scene->pri = plugin_calloc(1, sizeof(struct GamePri));

    struct GamePri* pri = (struct GamePri*)scene->pri;

    pri->map = sokoban_get_map();
    pri->records = sokoban_get_records();

    sokoban_load_map(sokoban_get_stage());
    pri->map_textrue[BOX] = plugin_load_texture(RES(box));
    pri->map_textrue[PLAYER] = plugin_load_texture(RES(player));
    pri->map_textrue[SPACE] = plugin_load_texture(RES(space));
    pri->map_textrue[TARGET] = plugin_load_texture(RES(target));
    pri->map_textrue[BLOCK] = plugin_load_texture(RES(block));

    Vector2 pos = game_pos_to_position(pri->map, find_player(pri->map));
    pri->camera = (Camera2D){
        {plugin_get_window_width() / 2, plugin_get_window_height() / 2},
        pos,
        0.0f,
        1.0f,
    };

    AnimValue v = {
        .type = ANIMATION_POSITION,
        .from.position = pos,
        .to.position = pos,
    };
    animation_init(&pri->animation, 0.25f, v, &pri->current_pos);
    anim_init(&pri->anim, ANIM_ANIMATION, &pri->animation);
    pri->index = -1;
    pri->anim.loop = false;
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

    plugin_begin_mode_2d(pri->camera);

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

        if (i == pri->index)
        {
            continue;
        }

        if (map_is(map, i, MAP_PLAYER))
            plugin_draw_texture(map_texture[PLAYER], x, y, WHITE);
        if (map_is(map, i, MAP_BOX))
            plugin_draw_texture(map_texture[BOX], x, y, WHITE);
    }

    if (pri->index != -1)
    {
        if (map_is(map, pri->index, MAP_PLAYER))
            plugin_draw_texture(map_texture[PLAYER], pri->current_pos.x, pri->current_pos.y, WHITE);
        if (map_is(map, pri->index, MAP_BOX))
            plugin_draw_texture(map_texture[BOX], pri->current_pos.x, pri->current_pos.y, WHITE);
    }

    // plugin_draw_rectangle(desired.x - 160, desired.y - 120, 360, 280, (Color){255, 109, 194, 125});
    plugin_end_mode_2d();
    char str[32];
    snprintf(str, sizeof(str), "step: %d", records_get_count(pri->records));
    plugin_draw_text(str, 0, 0, 20, RED);

    int count = 0;
    for (int i = 0; i < w * h; i++)
    {
        if (map_get(map, i) == MAP_BOX)
            count++;
    }
    snprintf(str, sizeof(str), "left: %d", count);
    plugin_draw_text(str, 0, 20, 20, RED);
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
    case OPTION_REDO:
        ret = CMD_REDO;
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

    anim_update(&pri->anim, 1.0 / 60);
    MovAction action;

    switch (c)
    {
    case OPTION_BACK:
        scene_manager_reset(scene->mgr, GAME_MENU);
        scene_manager_set_overlay(scene->mgr, GAME_MENU);
        break;
    default:
        action = core_update(pri->map, pri->records, key_to_command(c));
        if (action.direct == CMD_NONE)
            break;
        pri->animation.target.from.position = game_pos_to_position(pri->map, action.pos_f);
        pri->animation.target.to.position = game_pos_to_position(pri->map, action.pos_t);
        pri->current_pos = pri->animation.target.from.position;
        pri->index = action.pos_t;
        anim_reset(&pri->anim);
        break;
    }

    if (core_is_win(pri->map))
    {
        sokoban_set_result(1);
        scene_manager_set_overlay(scene->mgr, GAME_END);
    }

    Map* map = pri->map;

    int screen_width = plugin_get_window_width();
    int screen_height = plugin_get_window_height();

    int map_width = map->w * MAP_BLOCK_SIZE;
    int map_height = map->h * MAP_BLOCK_SIZE;

    Vector2 player = game_pos_to_position(map, find_player(map));
    Vector2 desired = pri->camera.target; // 默认不动

    float dx = player.x - desired.x;
    float dy = player.y - desired.y;

    if (dx >= DEADZONE_W)
        desired.x = player.x - DEADZONE_W;
    if (dx < -DEADZONE_W)
        desired.x = player.x + DEADZONE_W;
    if (dy >= DEADZONE_H)
        desired.y = player.y - DEADZONE_H;
    if (dy < -DEADZONE_H)
        desired.y = player.y + DEADZONE_H;

    if (map_width <= screen_width)
    {
        desired.x = map_width * 0.5f;
    }
    else
    {
        if (desired.x < screen_width / 2)
            desired.x = screen_width / 2;
        if (desired.x > map_width - screen_width / 2)
            desired.x = map_width - screen_width / 2;
    }
    if (map_height <= screen_height)
    {
        desired.y = map_height * 0.5f;
    }
    else
    {
        if (desired.y < screen_height / 2)
            desired.y = screen_height / 2;
        if (desired.y > map_height - screen_height / 2)
            desired.y = map_height - screen_height / 2;
    }

    pri->camera.target = desired;
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

    plugin_free(pri);
}

/** @brief 游戏界面操作函数集 */
const SceneOps game_ops = {
    .init = game_init,
    .reset = game_reset,
    .render = game_render,
    .update = game_update,
    .exit = game_exit
};