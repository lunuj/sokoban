/**
 * @file core.h
 * @brief 游戏核心模块头文件
 * @details 提供地图数据的加载和保存函数接口
 */

#pragma once

#include <stdbool.h>

#define MAP_MAX_SIZE 1024

#define MAP_SPACE (0)
#define MAP_BLOCK (1 << 0)
#define MAP_PLAYER (1 << 1)
#define MAP_TARGET (1 << 2)
#define MAP_BOX (1 << 3)
#define MAP_ALL (0xFF)

typedef struct Map
{
    int w;
    int h;
    char map[MAP_MAX_SIZE];
} Map;

/**
 * @brief 从二进制文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int load_map(const char* filename, Map* map);

/**
 * @brief 将地图保存为二进制文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int save_map(const char* filename, Map* map);

int map_get(Map* map, int pos);
void map_clean(Map* map);
void map_set(Map* map, int pos, int maptype);
void map_clr(Map* map, int pos, int maptype);
bool map_is(Map* map, int pos, int maptype);

int get_map_width(Map* map);
void set_map_width(Map* map, int w);
int get_map_height(Map* map);
void set_map_height(Map* map, int h);

int find_player(Map* map);