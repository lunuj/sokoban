#include "stage.h"

#include "raylib/raylib.h"

struct StagePri {
    int index;
};

static void stage_init(Scene *scene)
{
    scene->end = false;
    scene->ops = stage_get_ops();
    scene->pri = calloc(1, sizeof(struct StagePri));

    struct StagePri *pri = (struct StagePri *)scene->pri;

    pri->index = 0;
}

static void stage_reset(Scene *scene)
{
    struct StagePri *pri = (struct StagePri *)scene->pri;

    pri->index = 0;
}

static void stage_render(Scene *scene)
{
    struct StagePri *pri = (struct StagePri *)scene->pri;
    char tmp[32];

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            snprintf(tmp, sizeof(tmp), "%03d", i * 10 + j + 1);
            DrawRectangle(20 * j * 3, 20 * i, 20 * 3, 20, WHITE);
            DrawText(tmp, 20 * j * 3, 20 * i, 20, RED);
            if(i*10 + j == pri->index)
            {
                DrawRectangle(20 * j * 3, 20 * i, 20 * 3, 20, RED);
                DrawText(tmp, 20 * j * 3, 20 * i, 20, WHITE);   
            }
        }
    }
}

static void stage_update(Scene *scene, int key)
{
    struct StagePri *pri = (struct StagePri *)scene->pri;
    int index = pri->index;

    switch (key) {
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
        scene_set_current(GAME);
        scene->end = true;
        break;
    case OPTION_BACK:
        scene_set_current(TITLE);
        scene->end = true;
        break;
    default:
        break;
    }

    if(index < 0) index = 0;
    if(index >= 200) index = 199;
    pri->index = index;
}

static void stage_exit(Scene *scene)
{
    free(scene->pri);
}

static SceneOps stage_ops = {
    .init = stage_init,
    .reset = stage_reset,
    .render = stage_render,
    .update = stage_update,
    .exit = stage_exit
};

SceneOps stage_get_ops()
{
    return stage_ops;
}