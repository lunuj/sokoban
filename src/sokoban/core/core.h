#pragma once
#include <stdbool.h>

typedef struct Map Map;
typedef struct Records Records;

enum MapType
{
    SPACE,
    BLOCK,
    PLAYER,
    TARGET,
    BOX,
    MAX,
};

typedef enum CoreOps
{
    CMD_NONE,
    CMD_MOV_UP,
    CMD_MOV_DOWN,
    CMD_MOV_LEFT,
    CMD_MOV_RIGHT,
    CMD_UNDO,
    CMD_REDO,
    CMD_ENTER,
    CMD_BACK,
} CoreOps;

void core_update(Map* map, Records* records, int cmd);
bool core_is_win(Map* map);