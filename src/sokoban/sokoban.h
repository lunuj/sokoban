#pragma once
#include "platform/platform_plugin.h"

#define ARCHIVE_FILENAME "sokoban.archive"
typedef struct Map Map;
typedef struct Records Records;

/* 场景 ID */
enum SceneType
{
    TITLE = 0,
    STAGE,
    SETTING,
    EDITOR,
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
    OPTION_BRUSH_1,
    OPTION_BRUSH_2,
    OPTION_BRUSH_3,
    OPTION_BRUSH_4,
    OPTION_BRUSH_5,
    OPTION_BRUSH_6,
    OPTION_BRUSH_7,
    OPTION_MAX,
};

void sokoban_set_result(bool result);
bool sokoban_get_result();
void sokoban_set_stage(int stage);
int sokoban_get_stage();
Map* sokoban_get_map(void);
void sokoban_load_map(int stage);
Records* sokoban_get_records(void);
bool sokoban_load_archive(const char* filename);
bool sokoban_save_archive(const char* filename);