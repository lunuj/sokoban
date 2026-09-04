/**
 * @file cli.c
 * @brief 命令行界面实现模块
 * @details 提供地图数据的字符转换、显示和命令行输入功能
 */

#include <stdio.h>

#include "core.h"
#include "scene/scene.h"

/** @brief 障碍物字符 */
#define BLOCK '#'
/** @brief 空格字符 */
#define SPACE ' '
/** @brief 玩家字符 */
#define PLAYER 'A'
/** @brief 目标点字符 */
#define TARGET '_'
/** @brief 箱子字符 */
#define BOX 'X'
/** @brief 玩家在目标点上字符 */
#define PLAYER_ON_TARGET 'B'
/** @brief 箱子在目标点上字符 */
#define BOX_ON_TARGET 'O'

char c = 0;             /**< 临时字符变量 */
char line[128];         /**< 输入行缓冲区 */

/**
 * @brief 将地图类型转换为显示字符
 * @param maptype 地图类型（位掩码）
 * @return 对应的显示字符
 */
int map_to_cli(int maptype)
{
    int ch = 0;
    switch (maptype) {
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
int cli_to_map(int ch)
{
    int ret = 0;
    switch (ch) {
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
 * @brief 在命令行显示地图
 * @param map 地图结构体指针
 */
void cli_show(Map *map)
{
    for (int i = 0; i < map->h; i++) {
        for (int j = 0; j < map->w; j++) {
            printf("%c", map_to_cli(map->map[i * map->w + j]));
        }
        printf("\n");
    }
}

/**
 * @brief 从命令行获取用户输入
 * @return 对应的命令操作码
 * @details 读取一行输入，根据首字符转换为对应命令
 */
int cli_get()
{
    int ret = CMD_NONE;

    fgets(line, sizeof(line), stdin);
    switch (line[0]) {
    case 'q':
        ret = CMD_BACK;
        break;
    case 'e':
        ret = CMD_UNDO;
        break;
    case 'w':
        ret = CMD_MOV_UP;
        break;
    case 's':
        ret = CMD_MOV_DOWN;
        break;
    case 'a':
        ret = CMD_MOV_LEFT;
        break;
    case 'd':
        ret = CMD_MOV_RIGHT;
        break;
    default:
        break;
    }
    // TODO: 输入优化
    return ret;
}