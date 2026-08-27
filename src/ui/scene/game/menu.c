#include "menu.h"
#include "std.h"

#include "raylib/raylib.h"
#include "ui/scene.h"

#define MAX_NUM 4

enum MenuContext
{
    CONTINUE,
    RESUME,
    BACK_TO_HOME,
    EXIT,
};

char *menu_text[MAX_NUM] = {
    "continue game",
    "restore game",
    "back to home",
    "exit game",
};

struct MenuPri
{
    int index;
    char **text;
};

static void menu_init(Scene *scene)
{
    scene->end = false;
    scene->ops = menu_get_ops();
    scene->pri = calloc(1, sizeof(struct MenuPri));

    struct MenuPri *pri = (struct MenuPri *)scene->pri;
    pri->index = 0;
    pri->text = menu_text;
}

static void menu_reset(Scene *scene)
{
    struct MenuPri *pri = (struct MenuPri *)scene->pri;

    pri->index = 0;
}

static void menu_draw(Scene *scene)
{
    struct MenuPri *pri = (struct MenuPri *)scene->pri;

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    DrawRectangle(0, 0, screen_width, screen_height, (Color){0, 0, 0, 180});
    for (int i = 0; i < MAX_NUM; i++)
    {
        if (i == pri->index)
            DrawText(pri->text[i], 0, i * 20, 14, YELLOW);
        else
            DrawText(pri->text[i], 0, i * 20, 14, RED);
    }
}

static void menu_update(Scene *scene, int c)
{
    struct MenuPri *pri = (struct MenuPri *)scene->pri;

    int *index = &pri->index;
    switch (c)
    {
    case OPTION_MOV_UP:
    case OPTION_MOV_LEFT:
        *index = (*index + MAX_NUM - 1) % MAX_NUM;
        break;
    case OPTION_MOV_DOWN:
    case OPTION_MOV_RIGHT:
        *index = (*index + 1) % MAX_NUM;
        break;
    case OPTION_ENTER:
        switch (*index)
        {
        case CONTINUE:
            scene->end = true;
            break;
        case RESUME:
            scene_reset(GAME);
            scene->end = true;
            break;
        case BACK_TO_HOME:
            scene_reset(GAME_END);
            scene_set_overlay(GAME, GAME_END);
            scene_update(OPTION_LOSE);
            scene->end = true;
            break;
        case EXIT:
            set_current(TITLE);
            scene_set_status(TITLE, true);
            scene->end = true;
            break;
        default:
            break;
        }
        break;
    case OPTION_BACK:
        scene->end = true;
        break;
    default:
        break;
    }
}

static void menu_exit(Scene *scene) {}

static SceneOps menu = {
    .init = menu_init,
    .reset = menu_reset,
    .draw = menu_draw,
    .update = menu_update,
    .exit = menu_exit,
};

SceneOps menu_get_ops()
{
    return menu;
}