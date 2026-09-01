#include "raylib/raylib.h"
#include "scene.h"

#include "scene/game.h"
#include "scene/game/end.h"
#include "scene/game/menu.h"
#include "scene/title.h"

static int current;
static Scene scene_list[SCENE_MAX];

Scene *get_current()
{
    return scene_list + current;
}

static Scene *get_scene(int index)
{
    return scene_list + index;
}

void set_current(int index)
{
    current = index;
}

void scene_init()
{
    SceneOps ops = title_get_ops();
    ops.init(scene_list + TITLE);
    current = TITLE;

    ops = game_get_ops();
    ops.init(scene_list + GAME);
    ops = menu_get_ops();
    ops.init(scene_list + GAME_MENU);
    ops = end_get_ops();
    ops.init(scene_list + GAME_END);
}

void scene_reset(int index)
{
    Scene *scene = get_scene(index);

    scene->ops.reset(scene);
    scene->overlay = NULL;
    scene->end = false;
}

void scene_draw()
{
    Scene *scene = get_current();

    BeginDrawing();
    scene->ops.draw(scene);
    if (scene->overlay != NULL)
        scene->overlay->ops.draw(scene->overlay);
    EndDrawing();
}

int scene_key_map(int c)
{
    int ret = OPTION_NONE;
    switch (c)
    {
    case KEY_W:
    case KEY_UP:
        ret = OPTION_MOV_UP;
        break;
    case KEY_S:
    case KEY_DOWN:
        ret = OPTION_MOV_DOWN;
        break;
    case KEY_A:
    case KEY_LEFT:
        ret = OPTION_MOV_LEFT;
        break;
    case KEY_D:
    case KEY_RIGHT:
        ret = OPTION_MOV_RIGHT;
        break;
    case KEY_ESCAPE:
        ret = OPTION_BACK;
        break;
    case KEY_ENTER:
        ret = OPTION_ENTER;
        break;
    case KEY_E:
        ret = OPTION_UNDO;
        break;
    case KEY_R:
        ret = OPTION_RELOAD;
    default:
        break;
    }
    return ret;
}

void scene_update(int c)
{
    Scene *scene = get_current();

    if (scene->overlay != NULL)
    {
        if (scene->overlay->end)
        {
            scene->overlay = NULL;
        }
        else
        {
            scene = scene->overlay;
        }
    }

    if (!scene->end)
        scene->ops.update(scene, c);
}

void scene_set_status(int index, int status)
{
    Scene *scene = get_scene(index);

    scene->end = (bool)status;
}

bool scene_get_status(int index)
{
    Scene *scene = get_scene(index);

    return scene->end;
}

void scene_exit()
{
    for (int i = 0; i < SCENE_MAX; i++)
    {
        scene_list[i].ops.exit(scene_list + i);
    }
}

bool scene_is_end()
{
    Scene *scene = get_current();
    return scene->end;
}

void scene_set_overlay(int index, int overlay)
{
    Scene *scene = get_current();

    scene = get_scene(index);
    scene->overlay = get_scene(overlay);
}

void scene_clr_overlay(int index)
{
    Scene *scene = get_scene(index);

    scene->overlay = NULL;
}