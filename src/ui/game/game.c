/**
 * @file game.c
 * @brief 游戏主界面模块实现
 * @details 实现游戏核心玩法，包括地图渲染、玩家控制和游戏逻辑更新
 */

#include <stdlib.h>
#include "game.h"

#include "core.h"
#include "raylib/raylib.h"
#include "scene/scene.h"

#include "menu.h"

/** @brief 资源路径宏 */
#define RES(str) "res/img/" #str ".png"
/** @brief 地图块渲染大小（像素） */
#define MAP_BLOCK_SIZE 40

/**
 * @brief 游戏界面私有数据结构
 */
struct GamePri
{
    Map map;                    /**< 地图数据 */
    Texture2D map_textrue[MAX]; /**< 地图块纹理数组 */
};

/**
 * @brief 渲染单个地图块
 * @param pri 游戏私有数据指针
 * @param block 地图块类型（位掩码）
 * @param x 屏幕X坐标
 * @param y 屏幕Y坐标
 * @details 根据地图块类型叠加绘制对应的纹理
 */
static void show_block(struct GamePri *pri, int block, int x, int y)
{
    if ((block & MAP_SPACE) == MAP_SPACE)
        DrawTexture(pri->map_textrue[SPACE], x, y, WHITE);
    if ((block & MAP_TARGET) == MAP_TARGET)
        DrawTexture(pri->map_textrue[TARGET], x, y, WHITE);
    if ((block & MAP_BLOCK) == MAP_BLOCK)
        DrawTexture(pri->map_textrue[BLOCK], x, y, WHITE);
    if ((block & MAP_PLAYER) == MAP_PLAYER)
        DrawTexture(pri->map_textrue[PLAYER], x, y, WHITE);
    if ((block & MAP_BOX) == MAP_BOX)
        DrawTexture(pri->map_textrue[BOX], x, y, WHITE);
}

/**
 * @brief 游戏界面初始化
 * @param scene 场景指针
 * @details 加载地图和纹理资源
 */
static void game_init(Scene *scene)
{
    scene->end = false;
    scene->ops = game_get_ops();
    scene->pri = calloc(1, sizeof(struct GamePri));

    struct GamePri *pri = (struct GamePri *)scene->pri;

    load_map(&pri->map, 1);
    pri->map.is_recording = true;
    pri->map_textrue[BOX] = LoadTexture(RES(box));
    pri->map_textrue[PLAYER] = LoadTexture(RES(player));
    pri->map_textrue[SPACE] = LoadTexture(RES(space));
    pri->map_textrue[TARGET] = LoadTexture(RES(target));
    pri->map_textrue[BLOCK] = LoadTexture(RES(block));
}

/**
 * @brief 游戏界面重置
 * @param scene 场景指针
 * @details 重新加载地图并重置录制状态
 */
static void game_reset(Scene *scene)
{
    struct GamePri *pri = (struct GamePri *)scene->pri;

    load_map(&pri->map, 1);
    pri->map.is_recording = true;
}

/**
 * @brief 游戏界面渲染
 * @param scene 场景指针
 * @details 绘制所有地图块
 */
static void game_render(Scene *scene)
{
    struct GamePri *pri = (struct GamePri *)scene->pri;

    int w = pri->map.w;
    int h = pri->map.h;

    ClearBackground(BLANK);
    for (int i = 0; i < w * h; i++)
    {
        int pos_x = (i % w) * MAP_BLOCK_SIZE;
        int pos_y = (i / w) * MAP_BLOCK_SIZE;
        int block = pri->map.map[i];
        show_block(pri, block, pos_x, pos_y);
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
static void game_update(Scene *scene, int c)
{
    struct GamePri *pri = (struct GamePri *)scene->pri;

    switch (c)
    {
    case OPTION_BACK:
        scene_reset(GAME_MENU);
        scene_set_overlay(GAME, GAME_MENU);
        break;
    default:
        update(&pri->map, key_to_command(c));
        break;
    }

    if (is_win(&pri->map))
    {
        scene_set_overlay(GAME, GAME_END);
        scene_set_status(GAME_END, false);
        scene_update(OPTION_WIN);
    }
}

/**
 * @brief 游戏界面退出清理
 * @param scene 场景指针
 * @details 卸载纹理并释放内存
 */
static void game_exit(Scene *scene)
{
    struct GamePri *pri = (struct GamePri *)scene->pri;
    for (int i = 0; i < MAX; i++)
    {
        UnloadTexture(pri->map_textrue[i]);
    }

    free(pri);
}

/** @brief 游戏界面操作函数集 */
static SceneOps game_ops = {
    .init = game_init,
    .reset = game_reset,
    .render = game_render,
    .update = game_update,
    .exit = game_exit
};

/**
 * @brief 获取游戏界面操作函数集
 * @return SceneOps结构体
 */
SceneOps game_get_ops()
{
    return game_ops;
}