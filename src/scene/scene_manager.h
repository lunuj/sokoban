#pragma once

#include <stdbool.h>
#include "scene_action.h"

#define SCENE_STACK_MAX 16
#define REQUEST_STACK_MAX 16

typedef struct Scene Scene;
typedef struct SceneOps SceneOps;
typedef struct PlatformApi PlatformApi;

typedef struct SceneManager
{
    Scene* scenes;
    int scene_count;
    int scene_capacity;

    int stack[SCENE_STACK_MAX];
    int top;

    SceneAction request[REQUEST_STACK_MAX];
    int request_count;

    int overlay;

    bool quit_requested;
} SceneManager;

SceneManager* scene_manager_create(int scene_capacity);
void scene_manager_destroy(SceneManager* mgr);

void scene_manager_reset(SceneManager* mgr, int id);

void scene_manager_register(SceneManager* mgr, int id, const SceneOps* ops);

void scene_manager_push(SceneManager* mgr, int id);
void scene_manager_pop(SceneManager* mgr);

void scene_manager_replace(SceneManager* mgr, int id);

void scene_manager_set_overlay(SceneManager* mgr, int id);
void scene_manager_clear_overlay(SceneManager* mgr);
int scene_manager_get_overlay(SceneManager* mgr);

void scene_manager_render(SceneManager* mgr);
bool scene_manager_update(SceneManager* mgr, int key);

Scene* scene_manager_current(SceneManager* mgr);
int scene_manager_current_id(SceneManager* mgr);

bool scene_manager_is_quitting(SceneManager* mgr);

void scene_manager_request(SceneManager* mgr, SceneAction action);

void scene_manager_reload(SceneManager* mgr, int index, const SceneOps* ops);
