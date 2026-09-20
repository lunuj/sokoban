/**
 * @file core.c
 * @brief 游戏核心模块实现
 * @details 提供地图数据的加载和保存功能，支持文本和二进制格式
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "map.h"

/**
 * @brief 从二进制文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int load_map(const char* filename, Map* map)
{
    FILE* fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        perror("打开文件失败\n");
        return -1;
    }

    fread(map, sizeof(*map), 1, fp);

    fclose(fp);
    return 0;
}

/**
 * @brief 将地图保存为二进制文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int save_map(const char* filename, Map* map)
{
    FILE* fp = fopen(filename, "wb+");

    if (fp == NULL)
    {
        perror("打开文件失败\n");
        return -1;
    }

    fwrite(map, sizeof(*map), 1, fp);

    fclose(fp);
    return 0;
}

void map_set(Map* map, int pos, int maptype)
{
    assert(pos <= map->w * map->h);
    map->map[pos] |= maptype;
}

int map_get(Map* map, int pos)
{
    assert(pos < map->w * map->h);
    return map->map[pos];
}

void map_clean(Map* map)
{
    memset(map, 0, sizeof(*map));
}

void map_clr(Map* map, int pos, int maptype)
{
    assert(pos < map->w * map->h);
    map->map[pos] &= ~maptype;
}

bool map_is(Map* map, int pos, int maptype)
{
    assert(pos < map->w * map->h);
    return (map->map[pos] & maptype) == maptype;
}

int get_map_width(Map* map)
{
    return map->w;
}

void set_map_width(Map* map, int w)
{
    map->w = w;
}

int get_map_height(Map* map)
{
    return map->h;
}

void set_map_height(Map* map, int h)
{
    map->h = h;
}

int find_player(Map* map)
{
    for (int i = 0; i < map->h; i++)
    {
        for (int j = 0; j < map->w; j++)
        {
            if ((map->map[i * map->w + j] & MAP_PLAYER) == MAP_PLAYER)
            {
                return i * map->w + j;
            }
        }
    }

    return -1;
}