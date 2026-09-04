/**
 * @file core.c
 * @brief 游戏核心模块实现
 * @details 提供地图数据的加载和保存功能，支持文本和二进制格式
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "core.h"
#include "ui/cli.h"

/**
 * @brief 从文本文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 * @details 从文本文件读取地图数据，自动识别地图宽高
 */
int load_map_from_txt(const char *filename, Map *map)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("打开文件失败\n");
        return -1;
    }

    int ch = 0;
    int index = 0;

    memset(map, 0, sizeof(*map));

    while ((ch = fgetc(fp)) != EOF)
    {
        if (index >= MAP_MAX_SIZE)
        {
            memset(map, 0, sizeof(*map));
            break;
        }

        if (ch == '\n')
        {
            map->h++;
            if (map->w == 0)
                map->w = index - 1;
        }
        else
        {
            map->map[index++ - map->h] = cli_to_map(ch);
        }
    }

    assert(map->w * map->h == index - map->h);

    fclose(fp);

    return 0;
}

/**
 * @brief 从二进制文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int load_map_from_bin(const char *filename, Map *map)
{
    FILE *fp = fopen(filename, "rb");

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
 * @brief 将地图保存为文本文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 * @details 将地图数据以字符形式写入文本文件
 */
int save_map_to_txt(const char *filename, Map *map)
{
    char ch = 0;
    FILE *fp = fopen(filename, "w+");

    if (fp == NULL)
    {
        perror("打开文件失败\n");
        return -1;
    }

    for (int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            ch = map_to_cli(map->map[j * map->w + i]);
            fputc(ch, fp);
        }
        fputc('\n', fp);
    }

    return 0;
}

/**
 * @brief 将地图保存为二进制文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int save_map_to_bin(const char *filename, Map *map)
{
    FILE *fp = fopen(filename, "wb+");

    if (fp == NULL)
    {
        perror("打开文件失败\n");
        return -1;
    }

    fwrite(map, sizeof(*map), 1, fp);

    fclose(fp);
    return 0;
}