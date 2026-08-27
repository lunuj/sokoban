#include "std.h"

#include "core.h"
#include "tool/map.h"

Map map;

static inline void map_set(Map *map, int pos, int maptype)
{
    assert(pos < map->w * map->h);
    map->map[pos] |= maptype;
}

static inline int map_get(Map *map, int pos)
{
    assert(pos < map->w * map->h);
    return map->map[pos];
}

static inline void map_clear(Map *map, int pos, int maptype)
{
    assert(pos < map->w * map->h);
    map->map[pos] &= ~maptype;
}

static inline bool map_is(Map *map, int pos, int maptype)
{
    assert(pos < map->w * map->h);
    return (map->map[pos] & maptype) == maptype;
}

static inline int get_map_width(Map *map)
{
    return map->w;
}

static inline int get_map_height(Map *map)
{
    return map->h;
}

int find_player(Map *map)
{
    int w = get_map_width(map);
    int h = get_map_height(map);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if ((map->map[i * w + j] & MAP_PLAYER) == MAP_PLAYER)
            {
                return i * w + j;
            }
        }
    }

    return -1;
}

void records_add(Map *map, int pos_f, int direct, int step, int pos_t)
{
    assert(map->record_count < RECORD_SIZE && "record too big!!");
    if (map->is_recording)
    {
        map->records[map->record_count].pos_f = pos_f;
        map->records[map->record_count].direct = direct;
        map->records[map->record_count].step = step;
        map->records[map->record_count].pos_t = pos_t;
        map->record_count++;
    }
}

void mov(Map *map, int pos_f, int direct, int step)
{
    int w = get_map_width(map);
    int h = get_map_height(map);
    int pos_t = pos_f;

    switch (direct)
    {
    case CMD_MOV_UP:
        pos_t = pos_f - w * step;
        break;
    case CMD_MOV_DOWN:
        pos_t = pos_f + w * step;
        break;
    case CMD_MOV_LEFT:
        pos_t = pos_f - step;
        break;
    case CMD_MOV_RIGHT:
        pos_t = pos_f + step;
        break;
    default:
        break;
    }

    if (pos_t < 0 || pos_t >= w * h)
    {
        return;
    }

    char *f = map->map + pos_f;
    char *t = map->map + pos_t;

    if (map_is(map, pos_t, MAP_BLOCK))
    {
        return;
    }

    if (map_is(map, pos_t, MAP_BOX))
    {
        mov(map, pos_t, direct, step);
        return;
    }

    if (map_is(map, pos_f, MAP_PLAYER))
    {
        map_clear(map, pos_f, MAP_PLAYER);
        map_set(map, pos_t, MAP_PLAYER);
    }
    if (map_is(map, pos_f, MAP_BOX))
    {
        map_clear(map, pos_f, MAP_BOX);
        map_set(map, pos_t, MAP_BOX);
    }

    records_add(map, pos_f, direct, step, pos_t);
}

void records_undo(Map *map)
{
    if (map->record_count > 0)
    {
        map->record_count--;
        map->is_recording = false;
        int direct = map->records[map->record_count].direct;
        int step = map->records[map->record_count].step;
        int pos_t = map->records[map->record_count].pos_t;

        switch (direct)
        {
        case CMD_MOV_UP:
            mov(map, pos_t, CMD_MOV_DOWN, step);
            break;
        case CMD_MOV_DOWN:
            mov(map, pos_t, CMD_MOV_UP, step);
            break;
        case CMD_MOV_LEFT:
            mov(map, pos_t, CMD_MOV_RIGHT, step);
            break;
        case CMD_MOV_RIGHT:
            mov(map, pos_t, CMD_MOV_LEFT, step);
            break;
        default:
            break;
        }
        map->is_recording = true;
    }
}

int update(Map *map, int c)
{
    int pos = find_player(map);

    switch (c)
    {
    case CMD_MOV_UP:
    case CMD_MOV_DOWN:
    case CMD_MOV_LEFT:
    case CMD_MOV_RIGHT:
        mov(map, pos, c, 1);
        break;
    case CMD_UNDO:
        records_undo(map);
        break;
    case CMD_BACK:
        return -1;
    default:
        break;
    }

    return 0;
}

bool is_win(Map *map)
{
    for (int i = 0; i < get_map_height(map); i++)
    {
        for (int j = 0; j < get_map_width(map); j++)
        {
            if (map->map[i * get_map_width(map) + j] == MAP_BOX)
            {
                return false;
            }
        }
    }
    return true;
}

#define MAP_RES "res/map/map%02d.txt"

void load_map(Map *map, int index)
{
    (void)index;
    char str[32];
    snprintf(str, sizeof(str), MAP_RES, index);
    load_map_from_txt(str, map);
}