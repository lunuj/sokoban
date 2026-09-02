#pragma once

enum Option
{
    OPTION_NONE,
    OPTION_WIN,
    OPTION_LOSE,
    OPTION_BACK,
    OPTION_ENTER,
    OPTION_MOV_UP,
    OPTION_MOV_DOWN,
    OPTION_MOV_LEFT,
    OPTION_MOV_RIGHT,
    OPTION_UNDO,
    OPTION_RELOAD,
};

enum SceneType
{
    TITLE,
    GAME,
    GAME_MENU,
    GAME_END,
    SCENE_MAX,
};

typedef struct Scene Scene;

typedef struct SceneOps
{
    void (*init)(Scene *scene);
    void (*reset)(Scene *scene);
    void (*update)(Scene *scene, int c);
    void (*draw)(Scene *scene);
    void (*exit)(Scene *scene);
} SceneOps;

typedef struct Scene
{
    bool end;
    SceneOps ops;
    Scene *overlay;
    void *pri;
} Scene;

void scene_init();
void scene_reset(int index);
void scene_draw();
void scene_update(int c);
void scene_set_status(int index, int status);
bool scene_get_status(int index);
void scene_exit();

bool scene_is_end();

void scene_set_overlay(int index, int overlay);
void scene_clr_overlay(int index);

int scene_key_map(int c);

Scene *get_current();
void set_current(int index);
