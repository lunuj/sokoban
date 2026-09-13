#include "platform/platform_plugin.h"
#include "scene_action.h"
#include "scene_manager.h"
#include "scene_pri.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static Scene* get_scene(SceneManager* mgr, int id)
{
    assert(id >= 0 && id < mgr->scene_count);
    return &mgr->scenes[id];
}

static void apply_action(SceneManager* mgr)
{
    for (int i = 0; i < mgr->request_count; i++)
    {
        SceneAction* act = mgr->request + i;
        switch (act->type)
        {
        case SCENE_ACTION_NONE:
            break;
        case SCENE_ACTION_PUSH:
            scene_manager_set_overlay(mgr, act->target);
            break;

        case SCENE_ACTION_POP:
            scene_manager_pop(mgr);
            break;

        case SCENE_ACTION_REPLACE:
            scene_manager_replace(mgr, act->target);
            break;

        case SCENE_ACTION_QUIT:
            mgr->quit_requested = true;
            break;

        case SCENE_ACTION_RESET:
            scene_manager_reset(mgr, act->target);
            break;

        case SCENE_ACTION_COMMAND:
            break;

        default:
            break;
        }
    }
    mgr->request_count = 0;
}

SceneManager* scene_manager_create(int scene_capacity)
{
    assert(scene_capacity > 0);
    SceneManager* mgr = plugin_calloc(1, sizeof(SceneManager));
    if (!mgr)
        goto err_mgr;

    mgr->scenes = plugin_calloc(scene_capacity, sizeof(Scene));
    if (!mgr->scenes)
        goto err_scenes;

    mgr->scene_capacity = scene_capacity;
    mgr->scene_count = 0;

    mgr->top = -1;

    mgr->request_count = 0;

    mgr->overlay = -1;

    mgr->quit_requested = false;

    return mgr;

err_scenes:
    plugin_free(mgr);
err_mgr:
    return NULL;
}

void scene_manager_destroy(SceneManager* mgr)
{
    if (!mgr)
        return;

    if (mgr->overlay >= 0)
    {
        scene_exit(get_scene(mgr, mgr->overlay));
    }

    while (mgr->top >= 0)
    {
        int id = mgr->stack[mgr->top];
        scene_exit(get_scene(mgr, id));
        mgr->top--;
    }

    plugin_free(mgr->scenes);
    plugin_free(mgr);
}

void scene_manager_reset(SceneManager* mgr, int id)
{
    assert(mgr);
    assert(id >= 0 && id < mgr->scene_capacity);

    scene_reset(&mgr->scenes[id]);
}

void scene_manager_register(SceneManager* mgr, int id, const SceneOps* ops)
{
    assert(mgr);
    assert(id >= 0 && id < mgr->scene_capacity);
    assert(ops->init);

    Scene* s = &mgr->scenes[id];

    s->ops = ops;
    s->mgr = mgr;
    scene_init(s);

    if (id >= mgr->scene_count)
        mgr->scene_count = id + 1;
}

void scene_manager_push(SceneManager* mgr, int id)
{
    assert(mgr);
    assert(mgr->top < SCENE_STACK_MAX - 1 && "scene stack overflow");

    mgr->top++;
    mgr->stack[mgr->top] = id;
}

void scene_manager_pop(SceneManager* mgr)
{
    assert(mgr);
    if (mgr->top < 0)
        return;

    if (mgr->overlay >= 0)
    {
        mgr->overlay = -1;
        return;
    }

    mgr->top--;
}

void scene_manager_replace(SceneManager* mgr, int id)
{
    assert(mgr);

    if (mgr->overlay >= 0)
    {
        mgr->overlay = -1;
    }

    if (mgr->top >= 0)
    {
        mgr->top--;
    }

    scene_manager_push(mgr, id);
}

void scene_manager_set_overlay(SceneManager* mgr, int id)
{
    assert(mgr);
    mgr->overlay = id;
}

void scene_manager_clear_overlay(SceneManager* mgr)
{
    assert(mgr);
    mgr->overlay = -1;
}

int scene_manager_get_overlay(SceneManager* mgr)
{
    return mgr->overlay;
}

void scene_manager_render(SceneManager* mgr)
{
    assert(mgr);

    if (mgr->top >= 0)
    {
        int id = mgr->stack[mgr->top];
        scene_render(get_scene(mgr, id));
    }

    if (mgr->overlay >= 0)
    {
        scene_render(get_scene(mgr, mgr->overlay));
    }
}

bool scene_manager_update(SceneManager* mgr, int key)
{
    assert(mgr);

    Scene* active;
    if (mgr->overlay >= 0)
    {
        active = get_scene(mgr, mgr->overlay);
    }
    else if (mgr->top >= 0)
    {
        active = get_scene(mgr, mgr->stack[mgr->top]);
    }
    else
    {
        return mgr->quit_requested;
    }

    scene_update(active, key);
    apply_action(mgr);

    return mgr->quit_requested;
}

Scene* scene_manager_current(SceneManager* mgr)
{
    if (!mgr)
        return NULL;
    if (mgr->overlay >= 0)
        return get_scene(mgr, mgr->overlay);
    if (mgr->top >= 0)
        return get_scene(mgr, mgr->stack[mgr->top]);
    return NULL;
}

int scene_manager_current_id(SceneManager* mgr)
{
    if (!mgr)
        return -1;
    if (mgr->overlay >= 0)
        return mgr->overlay;
    if (mgr->top >= 0)
        return mgr->stack[mgr->top];
    return -1;
}

bool scene_manager_is_quitting(SceneManager* mgr)
{
    return mgr ? mgr->quit_requested : true;
}

void scene_manager_request(SceneManager* mgr, SceneAction action)
{
    assert(mgr->request_count < REQUEST_STACK_MAX);
    mgr->request[mgr->request_count] = action;
    mgr->request_count++;
}

void scene_manager_reload(SceneManager* mgr, int index, const SceneOps* ops)
{
    mgr->scenes[index].ops = ops;
}