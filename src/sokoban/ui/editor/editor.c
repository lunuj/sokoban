#include "editor.h"

#include <stdio.h>

#include "../../core/core.h"
#include "../../core/map.h"
#include "../../sokoban.h"
#include "../../tool/xsb.h"
#include "scene/scene_manager.h"

#include "anim.h"

#define MAP_BLOCK_SIZE 40
#define MAP_W 32
#define MAP_H 32
#define DEADZONE_W 160
#define DEADZONE_H 120
#define RES(str) "res/img/" #str ".png"

struct EditorPri
{
    Map map; /**< 地图数据 */
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

static void editor_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &editor_ops;
    scene->pri = plugin_calloc(1, sizeof(struct EditorPri));

    struct EditorPri* pri = (struct EditorPri*)scene->pri;

    pri->map.w = MAP_W;
    pri->map.h = MAP_H;

    pri->map_textrue[BOX] = plugin_load_texture(RES(box));
    pri->map_textrue[PLAYER] = plugin_load_texture(RES(player));
    pri->map_textrue[SPACE] = plugin_load_texture(RES(space));
    pri->map_textrue[TARGET] = plugin_load_texture(RES(target));
    pri->map_textrue[BLOCK] = plugin_load_texture(RES(block));

    pri->index = MAP_W * MAP_H / 2;
    Vector2 pos = game_pos_to_position(&pri->map, pri->index);
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
    pri->anim.loop = false;
}

static void editor_reset(Scene* scene)
{
    struct EditorPri* pri = (struct EditorPri*)scene->pri;
}

static void editor_render(Scene* scene)
{
    struct EditorPri* pri = (struct EditorPri*)scene->pri;
    Map* map = &pri->map;
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
        if (map_is(map, i, MAP_PLAYER))
            plugin_draw_texture(map_texture[PLAYER], x, y, WHITE);
        if (map_is(map, i, MAP_BOX))
            plugin_draw_texture(map_texture[BOX], x, y, WHITE);
    }

    plugin_draw_rectangle(
        pri->current_pos.x,
        pri->current_pos.y,
        MAP_BLOCK_SIZE,
        MAP_BLOCK_SIZE,
        (Color){255, 109, 194, 125}
    );
    plugin_end_mode_2d();
}

static void editor_save_map(Map* map)
{
    Map tmp;
    tmp.w = 0;
    tmp.h = 0;
    for (int j = 0; j < MAP_H; j++)
    {
        int index = 0;
        for (int i = 0; i < MAP_W; i++)
        {
            if (map->map[j * MAP_W + i] != MAP_SPACE)
                index = i + 1;
        }
        if (tmp.w < index)
            tmp.w = index;
        if (index > 0)
            tmp.h = j + 1;
    }

    for (int i = 0; i < tmp.w * tmp.h; i++)
    {
        int w = i % tmp.w;
        int h = i / tmp.w;
        tmp.map[i] = map->map[h * MAP_W + w];
    }
    save_map_to_xsb("res/map/map.xsb", &tmp);
}

static void editor_update(Scene* scene, int key)
{
    struct EditorPri* pri = (struct EditorPri*)scene->pri;
    Map* map = &pri->map;

    int index = pri->index;
    anim_update(&pri->anim, 1.0 / 60);

    switch (key)
    {
    case OPTION_MOV_UP:
        pri->index -= MAP_W;
        break;
    case OPTION_MOV_LEFT:
        pri->index -= 1;
        break;
    case OPTION_MOV_RIGHT:
        pri->index += 1;
        break;
    case OPTION_MOV_DOWN:
        pri->index += MAP_W;
        break;
    case OPTION_BACK:
        scene_manager_replace(scene->mgr, SETTING);
        break;
    case OPTION_BRUSH_1:
        map_clr(map, index, MAP_ALL);
        break;
    case OPTION_BRUSH_2:
        map_clr(map, index, MAP_ALL);
        map_set(map, index, MAP_BLOCK);
        break;
    case OPTION_BRUSH_3:
        map_clr(map, index, MAP_ALL);
        map_set(map, index, MAP_BOX);
        break;
    case OPTION_BRUSH_4:
        map_clr(map, index, MAP_ALL);
        while (find_player(map) >= 0)
            map_clr(map, find_player(map), MAP_ALL);
        map_set(map, index, MAP_PLAYER);
        break;
    case OPTION_BRUSH_5:
        map_clr(map, index, MAP_ALL);
        map_set(map, index, MAP_TARGET);
        break;
    case OPTION_ENTER:
        editor_save_map(map);
        break;
    default:
        break;
    }

    if (index != pri->index)
    {
        pri->index = (pri->index + MAP_MAX_SIZE) % MAP_MAX_SIZE;
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = game_pos_to_position(&pri->map, pri->index);
        anim_reset(&pri->anim);
    }

    int screen_width = plugin_get_window_width();
    int screen_height = plugin_get_window_height();

    int map_width = map->w * MAP_BLOCK_SIZE;
    int map_height = map->h * MAP_BLOCK_SIZE;

    Vector2 player = pri->current_pos;
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

static void editor_exit(Scene* scene)
{
    struct EditorPri* pri = (struct EditorPri*)scene->pri;
    for (int i = 0; i < MAX; i++)
    {
        plugin_unload_texture(pri->map_textrue[i]);
    }

    plugin_free(pri);
}

const SceneOps editor_ops = {
    .init = editor_init,
    .reset = editor_reset,
    .render = editor_render,
    .update = editor_update,
    .exit = editor_exit
};