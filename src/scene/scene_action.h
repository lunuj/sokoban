#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum SceneActionType
{
    SCENE_ACTION_NONE,
    SCENE_ACTION_PUSH,
    SCENE_ACTION_POP,
    SCENE_ACTION_REPLACE,
    SCENE_ACTION_QUIT,
    SCENE_ACTION_RESET,
    SCENE_ACTION_COMMAND,
} SceneActionType;

typedef struct SceneAction
{
    SceneActionType type;
    int target;

    void* payload;
    size_t payload_size;
} SceneAction;

static inline SceneAction scene_action_none(void)
{
    return (SceneAction){
        .type = SCENE_ACTION_NONE,
    };
}

static inline SceneAction scene_action_push(int target)
{
    return (SceneAction){
        .type = SCENE_ACTION_PUSH,
        .target = target,
    };
}

static inline SceneAction scene_action_pop(void)
{
    return (SceneAction){.type = SCENE_ACTION_POP};
}

static inline SceneAction scene_action_replace(int target)
{
    return (SceneAction){
        .type = SCENE_ACTION_REPLACE,
        .target = target,
    };
}

static inline SceneAction scene_action_quit(void)
{
    return (SceneAction){
        .type = SCENE_ACTION_QUIT,
    };
}

static inline SceneAction scene_action_reset(int target)
{
    return (SceneAction){
        .type = SCENE_ACTION_RESET,
        .target = target,
    };
}

static inline SceneAction scene_action_command(int target, void* payload, size_t size)
{
    return (SceneAction){
        .type = SCENE_ACTION_COMMAND,
        .target = target,
        .payload = payload,
        .payload_size = size,
    };
}