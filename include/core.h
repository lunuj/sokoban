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

enum Option {
    OPTION_NONE,
    OPTION_QUIT,
    OPTION_RECORD_BACK,
    OPTION_MOV_UP,
    OPTION_MOV_DOWN,
    OPTION_MOV_LEFT,
    OPTION_MOV_RIGHT,
};

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
