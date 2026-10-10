#include "stage.h"

#include <stdio.h>

#include "../sokoban.h"
#include "scene/scene_manager.h"

#include "anim.h"

struct StagePri
{
    int index;

    Anim anim;
    Animation animation;
    Vector2 current_pos; // 插值后的位置
};

static void stage_init(Scene* scene)
{
    scene->status = false;
    scene->ops = &stage_ops;
    scene->pri = plugin_calloc(1, sizeof(struct StagePri));

    struct StagePri* pri = (struct StagePri*)scene->pri;

    pri->index = 0;
    AnimValue v = {
        .type = ANIMATION_POSITION,
        .from.position = (Vector2){0, 0},
        .to.position = (Vector2){0, 0},
    };
    animation_init(&pri->animation, 0.25f, v, &pri->current_pos);
    anim_init(&pri->anim, ANIM_ANIMATION, &pri->animation);
    pri->anim.loop = false;
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
        }
    }

    /* TODO */
    /* 使用shader实现区域颜色翻转 */
    plugin_draw_rectangle(
        pri->current_pos.x, pri->current_pos.y, 20 * 3, 20, (Color){230, 41, 55, 125}
    );
}

static void stage_update(Scene* scene, int key)
{
    struct StagePri* pri = (struct StagePri*)scene->pri;
    int index = pri->index;
    anim_update(&pri->anim, 1.0 / 60);

    switch (key)
    {
    case OPTION_MOV_UP:
        index = (index - 10);
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = (Vector2){20 * (index % 10) * 3, 20 * (index / 10)};
        anim_reset(&pri->anim);
        break;
    case OPTION_MOV_DOWN:
        index = (index + 10);
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = (Vector2){20 * (index % 10) * 3, 20 * (index / 10)};
        anim_reset(&pri->anim);
        break;
    case OPTION_MOV_LEFT:
        index = (index - 1);
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = (Vector2){20 * (index % 10) * 3, 20 * (index / 10)};
        anim_reset(&pri->anim);
        break;
    case OPTION_MOV_RIGHT:
        index = (index + 1);
        pri->animation.target.from.position = pri->current_pos;
        pri->animation.target.to.position = (Vector2){20 * (index % 10) * 3, 20 * (index / 10)};
        anim_reset(&pri->anim);
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