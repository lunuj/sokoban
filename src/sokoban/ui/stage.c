#include "stage.h"

#include <stdio.h>

#include "../sokoban.h"
#include "scene/scene_manager.h"

struct StagePri
{
    int index;
};

static void stage_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &stage_ops;
    scene->pri = plugin_calloc(1, sizeof(struct StagePri));

    struct StagePri* pri = (struct StagePri*)scene->pri;

    pri->index = 0;
}

static void stage_reset(Scene* scene)
{
    struct StagePri* pri = (struct StagePri*)scene->pri;

    pri->index = 0;
}

static void stage_render(Scene* scene)
{
    struct StagePri* pri = (struct StagePri*)scene->pri;
    char tmp[32];

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            snprintf(tmp, sizeof(tmp), "%03d", i * 10 + j + 1);
            plugin_draw_rectangle(20 * j * 3, 20 * i, 20 * 3, 20, WHITE);
            plugin_draw_text(tmp, 20 * j * 3, 20 * i, 20, RED);
            if (i * 10 + j == pri->index)
            {
                plugin_draw_rectangle(20 * j * 3, 20 * i, 20 * 3, 20, RED);
                plugin_draw_text(tmp, 20 * j * 3, 20 * i, 20, WHITE);
            }
        }
    }
}

static void stage_update(Scene* scene, int key)
{
    struct StagePri* pri = (struct StagePri*)scene->pri;
    int index = pri->index;

    switch (key)
    {
    case OPTION_MOV_UP:
        index = (index - 10);
        break;
    case OPTION_MOV_DOWN:
        index = (index + 10);
        break;
    case OPTION_MOV_LEFT:
        index = (index - 1);
        break;
    case OPTION_MOV_RIGHT:
        index = (index + 1);
        break;
    case OPTION_ENTER:
        sokoban_set_stage(index + 1);
        scene_manager_reset(scene->mgr, GAME);
        scene_manager_replace(scene->mgr, GAME);
        scene->status = true;
        break;
    case OPTION_BACK:
        scene_manager_replace(scene->mgr, TITLE);
        scene->status = true;
        break;
    default:
        break;
    }

    if (index < 0)
        index = 0;
    if (index >= 200)
        index = 199;
    pri->index = index;
}

static void stage_exit(Scene* scene)
{
    plugin_free(scene->pri);
}

const SceneOps stage_ops = {
    .init = stage_init,
    .reset = stage_reset,
    .render = stage_render,
    .update = stage_update,
    .exit = stage_exit
};