#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include "raylib/raylib.h"

typedef enum AnimType
{
    ANIM_ANIMATION,
    ANIM_SEQUENCE,
    ANIM_PARALLEL,
    ANIM_LOOP,
} AnimType;

typedef enum
{
    ANIM_IDLE,
    ANIM_RUNNING,
    ANIM_PAUSED,
    ANIM_FINISHED,
    ANIM_CANCELLED
} AnimState;

typedef enum AnimationType
{
    ANIMATION_LINE = 0,
    ANIMATION_POSITION = 1,
    ANIMATION_COLOR = 2,
    ANIMATION_RECTANGLE = 3,
} AnimationType;

typedef struct
{
    AnimationType type;

    union
    {
        float scale;
        Vector2 position;
        Color color;
        Rectangle rectangle;
    } from, to;
} AnimValue;

typedef float (*Curve)(float w);
typedef void (*Interpolator)(AnimValue* target, float p, void* userdata);

typedef struct Anim Anim;

typedef struct AnimOps
{
    void (*start)(Anim*);
    void (*pause)(Anim*);
    void (*reset)(Anim*);
    void (*update)(Anim*, float);
    bool (*finished)(Anim*);
} AnimOps;

typedef struct
{
    Anim* children;
    int count;
} Sequence;

typedef struct
{
    Anim* children;
    int count;
} Parallel;

typedef struct
{
    float elapsed;
    float duration;
    Curve curve;

    AnimValue target;
    Interpolator interpolator;

    void* userdata;
} Animation;

struct Anim
{
    AnimType type;
    AnimState state;
    AnimOps* ops;
    bool loop;
    void* userdata;
};

void anim_init(Anim* node, AnimType type, void* userdata);
void anim_update(Anim* node, float dt);
void anim_reset(Anim* node);

void animation_update(Animation* anim, float dt);
void animation_init(Animation* anim, float duration, AnimValue target, void* userdata);
