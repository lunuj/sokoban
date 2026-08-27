#include "title.h"

#include "raylib/raylib.h"
#include "ui/scene.h"

#define RES(str) "res/ui/gui/" #str
#define SELECT_NUM 4

enum TitleContext
{
    NEW,
    SELECT,
    SETTING,
    EXIT,
};

static Vector2 pos[SELECT_NUM] = {
    {200, 150},
    {200, 190},
    {200, 240},
    {200, 290},
};

struct TitlePri
{
    Texture2D bg;
    int index;
    Texture2D select;
};

static void title_init(Scene *scene)
{
    scene->end = false;
    scene->ops = title_get_ops();
    scene->pri = calloc(1, sizeof(struct TitlePri));

    struct TitlePri *pri = (struct TitlePri *)scene->pri;

    pri->bg = LoadTexture(RES(title.png));
    pri->index = 0;
    pri->select = LoadTexture(RES(select.png));
}

static void title_reset(Scene *scene)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;
    pri->index = 0;
}

static void title_draw(Scene *scene)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    ClearBackground(WHITE);
    DrawTexturePro(
        pri->bg,
        (Rectangle){0, 0, pri->bg.width, pri->bg.height},
        (Rectangle){0, 0, screen_width, screen_height},
        (Vector2){0, 0},
        0,
        WHITE
    );
    DrawTexturePro(
        pri->select,
        (Rectangle){0, 0, pri->select.width, pri->select.height},
        (Rectangle){pos[pri->index].x, pos[pri->index].y, pri->select.width, pri->select.height},
        (Vector2){0, 0},
        0,
        WHITE
    );
}

static void title_exit(Scene *scene)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;
    UnloadTexture(pri->select);
    UnloadTexture(pri->bg);

    free(pri);
}

static void title_update(Scene *scene, int c)
{
    struct TitlePri *pri = (struct TitlePri *)scene->pri;

    int *index = &pri->index;
    switch (c)
    {
    case OPTION_MOV_UP:
    case OPTION_MOV_LEFT:
        *index = (*index + SELECT_NUM - 1) % SELECT_NUM;
        break;
    case OPTION_MOV_DOWN:
    case OPTION_MOV_RIGHT:
        *index = (*index + 1) % SELECT_NUM;
        break;
    case OPTION_ENTER:
        switch (*index)
        {
        case NEW:
            scene_reset(GAME);
            set_current(GAME);
            break;
        case SELECT:
            break;
        case SETTING:
            break;
        case EXIT:
            scene_set_status(TITLE, true);
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

static SceneOps title_ops = {
    .init = title_init,
    .reset = title_reset,
    .draw = title_draw,
    .update = title_update,
    .exit = title_exit,
};

SceneOps title_get_ops()
{
    return title_ops;
}