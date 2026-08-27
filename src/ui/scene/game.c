#include "game.h"
#include "std.h"

#include "core.h"
#include "raylib/raylib.h"
#include "ui/scene.h"

#include "game/menu.h"

#define RES(str) "res/img/" #str ".png"
#define MAP_BLOCK_SIZE 40

struct GamePri
{
    Map map;
    Texture2D map_textrue[MAX];
};

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
    pri->map_textrue[SPACE] = LoadTexture(RES(spcae));
    pri->map_textrue[TARGET] = LoadTexture(RES(target));
    pri->map_textrue[BLOCK] = LoadTexture(RES(block));
}

static void game_reset(Scene *scene)
{
    struct GamePri *pri = (struct GamePri *)scene->pri;

    load_map(&pri->map, 1);
    pri->map.is_recording = true;
}

static void game_draw(Scene *scene)
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

static void game_exit(Scene *scene)
{
    struct GamePri *pri = (struct GamePri *)scene->pri;
    for (int i = 0; i < MAX; i++)
    {
        UnloadTexture(pri->map_textrue[i]);
    }

    free(pri);
}

static SceneOps game_ops = {
    .init = game_init,
    .reset = game_reset,
    .draw = game_draw,
    .update = game_update,
    .exit = game_exit
};

SceneOps game_get_ops()
{
    return game_ops;
}
