#pragma once
#define MAP_WIDTH 10
#define MAP_HEIGHT 10

#define MAP_MAX_SIZE 120

#define MAP_SPACE (0)
#define MAP_BLOCK (1<<0)
#define MAP_PLAYER (1<<1)
#define MAP_TARGET (1<<2)
#define MAP_BOX (1<<3)

#define RECORD_SIZE 1024

enum MapType {
    SPACE,
    BLOCK,
    PLAYER,
    TARGET,
    BOX,
    MAX,
};

typedef enum CoreOps {
    CMD_NONE,
    CMD_MOV_UP,
    CMD_MOV_DOWN,
    CMD_MOV_LEFT,
    CMD_MOV_RIGHT,
    CMD_UNDO,
    CMD_ENTER,
    CMD_BACK,
} CoreOps;

typedef struct Record {
    int pos_f;
    int direct;
    int step;
    int pos_t;
} Record;

typedef struct Map {
    int w;
    int h;
    char map[MAP_MAX_SIZE];
    bool is_recording;
    int record_count;
    Record records[RECORD_SIZE];
} Map;

bool is_win(Map *map);
int update(Map *map, int c);
void mov(Map *map, int pos_f, int direct, int step);
int find_player(Map *map);

void records_add(Map *map, int pos_f, int direct, int step, int pos_t);
void records_undo(Map *map);

void load_map(Map *map, int index);
