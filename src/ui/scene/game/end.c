#include <stdlib.h>
#include "end.h"

#include "raylib/raylib.h"
#include "ui/scene.h"

#include "core.h"

#define MAX_NUM 2

char *end_text[MAX_NUM] = {
    "You Win !!",
    "You Lose !!",
};

struct EndPri
{
    int index;
    char **text;
};

static void end_init(Scene *scene)
{
    scene->end = false;
    scene->ops = end_get_ops();
    scene->pri = calloc(1, sizeof(struct EndPri));

    struct EndPri *pri = (struct EndPri *)scene->pri;
    pri->index = 0;
    pri->text = end_text;
}

static void end_reset(Scene *scene)
{
    struct EndPri *pri = (struct EndPri *)scene->pri;

    pri->index = 0;
}

static void end_render(Scene *scene)
{
    struct EndPri *pri = (struct EndPri *)scene->pri;

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    DrawRectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 180});
    DrawText(
        pri->text[pri->index],
        screen_width / 2,
        screen_height / 2,
        64,
        pri->index == 1 ? YELLOW : RED
    );
}

static void end_update(Scene *scene, int c)
{
    struct EndPri *pri = (struct EndPri *)scene->pri;

    switch (c)
    {
    case OPTION_WIN:
        pri->index = 0;
        break;
    case OPTION_LOSE:
        pri->index = 1;
        break;
    case OPTION_NONE:
        break;
    default:
        scene_set_current(TITLE);
        scene->end = true;
        break;
    }
}

static void end_exit(Scene *scene) {
    (void)scene;
}

static SceneOps end = {
    .init = end_init,
    .reset = end_reset,
    .render = end_render,
    .update = end_update,
    .exit = end_exit,
};

SceneOps end_get_ops()
{
    return end;
}