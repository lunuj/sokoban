#pragma once
#include "platform/platform_plugin.h"

typedef struct Map Map;
typedef struct Records Records;

/* 场景 ID */
enum SceneType
{
    TITLE = 0,
    STAGE,
    SETTING,
    GAME,
    GAME_MENU,
    GAME_END,
    SCENE_MAX,
};

/* 输入动作 */
enum Option
{
    OPTION_NONE,
    OPTION_BACK,
    OPTION_ENTER,
    OPTION_MOV_UP,
    OPTION_MOV_DOWN,
    OPTION_MOV_LEFT,
    OPTION_MOV_RIGHT,
    OPTION_UNDO,
    OPTION_REDO,
    OPTION_RELOAD,
    OPTION_MAX,
};

void sokoban_set_result(bool result);
bool sokoban_get_result();
void sokoban_set_stage(int stage);
int sokoban_get_stage();
Map* sokoban_get_map(void);
void sokoban_load_map(int stage);
Records* sokoban_get_records(void);