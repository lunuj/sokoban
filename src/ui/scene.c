#include <stddef.h>

#include "raylib/raylib.h"
#include "scene.h"

#include "scene/game.h"
#include "scene/game/end.h"
#include "scene/game/menu.h"
#include "scene/title.h"

#include "plugin/plugin.h"

struct PluginPri {
    uint32_t magic;
    Version verion;
    PlatformServices *svc;

    int current;
    Scene scene_list[SCENE_MAX];
} *pri;

Scene *get_current()
{
    return pri->scene_list + pri->current;
}

static Scene *get_scene(int index)
{
    return pri->scene_list + index;
}

void set_current(int index)
{
    pri->current = index;
}

// TODO 子场景下函数调用，资源申请问题
void scene_init(PlatformServices *service)
{
    pri = service->calloc(1, sizeof(PluginPri));
    pri->magic = 0xdeadbeef;
    pri->verion = version_get_current();
    pri->svc = service;

    SceneOps ops = title_get_ops();
    ops.init(pri->scene_list + TITLE);
    pri->current = TITLE;

    ops = game_get_ops();
    ops.init(pri->scene_list + GAME);
    ops = menu_get_ops();
    ops.init(pri->scene_list + GAME_MENU);
    ops = end_get_ops();
    ops.init(pri->scene_list + GAME_END);
}

void scene_reset(int index)
{
    Scene *scene = get_scene(index);

    scene->ops.reset(scene);
    scene->overlay = NULL;
    scene->end = false;
}

void scene_render()
{
    Scene *scene = get_current();

    BeginDrawing();
    scene->ops.draw(scene);
    if (scene->overlay != NULL) scene->overlay->ops.draw(scene->overlay);
    EndDrawing();
}

int scene_key_map(int c)
{
    int ret = OPTION_NONE;
    switch (c) {
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

void scene_update(int key)
{
    Scene *scene = get_current();

    if (scene->overlay != NULL) {
        if (scene->overlay->end) {
            scene->overlay = NULL;
        } else {
            scene = scene->overlay;
        }
    }

    if (scene->end)
        pri->svc->window_set_close();
    else
        scene->ops.update(scene, key);
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
    for (int i = 0; i < SCENE_MAX; i++) {
        pri->scene_list[i].ops.exit(pri->scene_list + i);
    }
    pri->svc->free(pri);
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

PluginPri *scene_pre_reload(void)
{
    return pri;
}

bool scene_post_reload(PluginPri *plugin)
{
    pri = plugin;
    Version v = version_get_current();
    pri->svc->log(7, "current version: %s\n", version_get_string(&v));
    pri->svc->log(7, "plugin version: %s\n", version_get_string(&pri->verion));
    return version_is_compatible(&v, &pri->verion);
}

PluginApi PLUGIN_API_NAME = {
    .name = "scene",
    .version = VERSION,

    .init = scene_init,
    .exit = scene_exit,

    .pre_reload = scene_pre_reload,
    .post_reload = scene_post_reload,

    .render = scene_render,
    .update = scene_update,

    .key_map = scene_key_map,
};