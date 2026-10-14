#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../core/map.h"
#include "xsb.h"

/** @brief 障碍物字符 */
#define BLOCK '#'
/** @brief 空格字符 */
#define SPACE ' '
/** @brief 玩家字符 */
#define PLAYER '@'
/** @brief 目标点字符 */
#define TARGET '.'
/** @brief 箱子字符 */
#define BOX '$'
/** @brief 玩家在目标点上字符 */
#define PLAYER_ON_TARGET '+'
/** @brief 箱子在目标点上字符 */
#define BOX_ON_TARGET '*'

/**
 * @brief 将地图类型转换为显示字符
 * @param maptype 地图类型（位掩码）
 * @return 对应的显示字符
 */
int map_to_xsb(int maptype)
{
    int ch = 0;
    switch (maptype)
    {
    case MAP_BLOCK:
        ch = BLOCK;
        break;
    case MAP_SPACE:
        ch = SPACE;
        break;
    case MAP_TARGET:
        ch = TARGET;
        break;
    case MAP_PLAYER:
        ch = PLAYER;
        break;
    case MAP_BOX:
        ch = BOX;
        break;
    case MAP_PLAYER | MAP_TARGET:
        ch = PLAYER_ON_TARGET;
        break;
    case MAP_BOX | MAP_TARGET:
        ch = BOX_ON_TARGET;
        break;
    default:
        ch = SPACE;
        break;
    }

    return ch;
}

/**
 * @brief 将显示字符转换为地图类型
 * @param ch 显示字符
 * @return 对应的地图类型（位掩码）
 */
int xsb_to_map(int ch)
{
    int ret = 0;
    switch (ch)
    {
    case BLOCK:
        ret = MAP_BLOCK;
        break;
    case SPACE:
        ret = MAP_SPACE;
        break;
    case TARGET:
        ret = MAP_TARGET;
        break;
    case PLAYER:
        ret = MAP_PLAYER;
        break;
    case BOX:
        ret = MAP_BOX;
        break;
    case PLAYER_ON_TARGET:
        ret = MAP_PLAYER | MAP_TARGET;
        break;
    case BOX_ON_TARGET:
        ret = MAP_BOX | MAP_TARGET;
        break;
    default:
        ret = MAP_SPACE;
        break;
    }

    return ret;
}

/**
 * @brief 将地图保存为文本文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 * @details 将地图数据以字符形式写入文本文件
 */
int save_map_to_xsb(const char* filename, Map* map)
{
    char ch = 0;
    FILE* fp = fopen(filename, "w+");

    if (fp == NULL)
    {
        perror("打开文件失败\n");
        return -1;
    }

    int w = get_map_width(map);
    int h = get_map_height(map);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            ch = map_to_xsb(map_get(map, j * w + i));
            fputc(ch, fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);

    return 0;
}

/**
 * @brief 从文本文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 * @details 从文本文件读取地图数据，自动识别地图宽高
 */
int load_map_from_xsb(const char* filename, Map* map)
{
    char buf[MAP_MAX_SIZE];
    FILE* fp = fopen(filename, "r");
    size_t ret = 0;

    if (fp == NULL)
    {
        perror(filename);
        return -1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);

    int h = 0;
    int w = 0;
    int max_width = 0;

    map_clean(map);

    if (buf[ret - 1] != '\n')
        buf[ret] = '\n';

    for (size_t i = 0; i <= ret; i++)
    {
        switch (buf[i])
        {
        case '\r':
            break;
        case '\n':
            if (max_width < w)
                max_width = w;
            w = 0;
            h++;
            break;
        default:
            w++;
            break;
        }
    }

    set_map_width(map, max_width);
    set_map_height(map, h);

    int ch = 0;
    int index = 0;

    for (size_t i = 0; i <= ret; i++)
    {
        ch = buf[i];
        if (ch == '\r' || ch == '\n')
        {
            if (index % max_width != 0)
            {
                int left = max_width - index % max_width;

                for (int j = 0; j < left; j++)
                    map_set(map, index++, xsb_to_map(SPACE));
            }
        }
        else
        {
            map_set(map, index++, xsb_to_map(ch));
        }
    }

    return 0;
}