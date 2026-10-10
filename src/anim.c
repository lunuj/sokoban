#include <assert.h>
#include <stddef.h>
#include "anim.h"

#pragma region "Curve"

static float animation_curve_line(float dt)
{
    return 3 * dt * dt - 2 * dt * dt * dt;
}

static Curve animation_get_curve(AnimationType type)
{
    switch (type)
    {
    case ANIMATION_POSITION:
        return animation_curve_line;
    default:
        return animation_curve_line;
    }
}

#pragma endregion

#pragma region "Interpolator"

#define Lerp(from, to, w) ((from) - (from) * (w) + (to) * (w))

// #define Lerp(from, to, w) ((w))

static void animation_interpolator_position(AnimValue* target, float p, void* userdata)
{
    assert(target->type == ANIMATION_POSITION);

    *(Vector2*)userdata = (Vector2){
        Lerp(target->from.position.x, target->to.position.x, p),
        Lerp(target->from.position.y, target->to.position.y, p),
    };
}

static void animation_interpolator_color(AnimValue* target, float p, void* userdata)
{
    assert(target->type == ANIMATION_COLOR);
    Color* color = (Color*)userdata;
    color->a = Lerp(target->from.color.a, target->to.color.a, p);
    color->r = Lerp(target->from.color.r, target->to.color.r, p);
    color->g = Lerp(target->from.color.g, target->to.color.g, p);
    color->b = Lerp(target->from.color.b, target->to.color.b, p);
}

static Interpolator animation_get_interpolator(AnimationType type)
{
    switch (type)
    {
    case ANIMATION_POSITION:
        return animation_interpolator_position;
    case ANIMATION_COLOR:
        return animation_interpolator_color;
    default:
        return animation_interpolator_position;
    }
}

#pragma endregion

#pragma region "Animation"

void animation_update(Animation* anim, float dt)
{
    assert(anim);

    anim->elapsed += dt;

    if (anim->elapsed >= anim->duration)
    {
        anim->elapsed = anim->duration;
        // TODO
        // anim->isfinished = true;
    }

    assert(anim->curve);
    float p = anim->curve(anim->elapsed / anim->duration);

    assert(anim->interpolator);
    anim->interpolator(&anim->target, p, anim->userdata);
}

void animation_reset(Animation* anim)
{
    assert(anim);
    anim->elapsed = 0;
}

void animation_init(Animation* anim, float duration, AnimValue target, void* userdata)
{
    assert(anim);
    anim->elapsed = 0;
    anim->duration = duration;
    anim->curve = animation_get_curve(target.type);

    anim->target = target;
    anim->interpolator = animation_get_interpolator(target.type);

    anim->userdata = userdata;
    // TODO
    // anim->isfinished = true;
}

bool animation_isfinished(Animation* anim)
{
    assert(anim);

    return anim->elapsed >= anim->duration;
}

#pragma endregion

#pragma region "Anim Node"

static inline void anim_animation_reset(Anim* node)
{
    assert(node);
    animation_reset((Animation*)node->userdata);
}

static inline bool anim_animation_isfinished(Anim* node)
{
    assert(node);
    return animation_isfinished((Animation*)node->userdata);
}

static inline void anim_animation_update(Anim* node, float dt)
{
    assert(node);
    animation_update((Animation*)node->userdata, dt);
    if (node->loop && anim_animation_isfinished(node))
    {
        anim_animation_reset(node);
    }
}

static inline void anim_sequence_reset(Anim* node)
{
    assert(node);
    assert(node->userdata);

    Sequence* s = (Sequence*)node->userdata;
    for (int i = 0; i < s->count; i++)
    {
        Anim a = s->children[i];
        assert(a.ops);
        assert(a.ops->reset);

        a.ops->reset(&a);
    }
}

static void anim_sequence_update(Anim* node, float dt)
{
    assert(node);
    assert(node->userdata);
    assert(node->ops->reset);

    Sequence* s = (Sequence*)node->userdata;
    for (int i = 0; i < s->count; i++)
    {
        Anim* a = s->children + i;
        assert(a->ops);
        assert(a->ops->finished);

        if (!a->ops->finished(a))
        {
            a->ops->update(a, dt);
            break;
        }
    }

    if (node->loop && node->ops->finished(node))
    {
        node->ops->reset(node);
    }
}

static bool anim_sequence_isfinished(Anim* node)
{
    assert(node);
    assert(node->userdata);

    Sequence* s = (Sequence*)node->userdata;
    for (int i = 0; i < s->count; i++)
    {
        Anim* a = s->children + i;
        assert(a->ops);
        assert(a->ops->finished);

        if (!a->ops->finished(a))
        {
            return false;
        }
    }

    return true;
}

static inline void anim_parallel_reset(Anim* node)
{
    assert(node);
    assert(node->userdata);

    Parallel* s = (Parallel*)node->userdata;
    for (int i = 0; i < s->count; i++)
    {
        Anim a = s->children[i];
        assert(a.ops);
        assert(a.ops->reset);

        a.ops->reset(&a);
    }
}

static inline void anim_parallel_update(Anim* node, float dt)
{
    assert(node);
    assert(node->userdata);
    assert(node->ops->reset);

    Parallel* p = (Parallel*)node->userdata;
    for (int i = 0; i < p->count; i++)
    {
        Anim* a = p->children + i;
        assert(a->ops);
        assert(a->ops->finished);

        if (!a->ops->finished(a))
        {
            a->ops->update(a, dt);
        }
    }

    if (node->loop && node->ops->finished(node))
    {
        node->ops->reset(node);
    }
}

static bool anim_parallel_isfinished(Anim* node)
{
    assert(node);
    assert(node->userdata);

    Parallel* p = (Parallel*)node->userdata;
    for (int i = 0; i < p->count; i++)
    {
        Anim* a = p->children + i;
        assert(a->ops);
        assert(a->ops->finished);

        if (!a->ops->finished(a))
        {
            return false;
        }
    }

    return true;
}

static AnimOps ops[] = {
    {NULL, NULL, anim_animation_reset, anim_animation_update, anim_animation_isfinished},
    {NULL, NULL, anim_sequence_reset, anim_sequence_update, anim_sequence_isfinished},
    {NULL, NULL, anim_parallel_reset, anim_parallel_update, anim_parallel_isfinished},
};

void anim_init(Anim* node, AnimType type, void* userdata)
{
    assert(node);
    assert(type < sizeof(ops) / sizeof(ops[0]));
    node->type = type;
    node->ops = &ops[type];
    node->userdata = userdata;
}

void anim_update(Anim* node, float dt)
{
    assert(node);
    assert(node->ops);
    assert(node->ops->update);
    node->ops->update(node, dt);
}

void anim_reset(Anim* node)
{
    assert(node);
    assert(node->ops);
    assert(node->ops->reset);
    node->ops->reset(node);
}

bool anim_is_finished(Anim* node)
{
    assert(node);
    assert(node->ops);
    assert(node->ops->finished);
    return node->ops->finished(node);
}

#pragma endregion