#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include "core.h"

#define BLOCK '#'
#define SPACE ' '
#define PLAYER 'A'
#define TARGET '_'
#define BOX 'X'
#define PLAYER_ON_TARGET 'B'
#define BOX_ON_TARGET 'O'

char c = 0;
char line[128];

int map_to_cli(int maptype)
{
    int ch = 0;
    switch(maptype)
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

int cli_to_map(int ch)
{
    int ret = 0;
    switch(ch)
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

void cli_show(Map *map)
{
    for(int i = 0; i < map->h; i++)
    {
        for(int j = 0; j < map->w; j++)
        {
            printf("%c", map_to_cli(map->map[i * map->w + j]));
        }
        printf("\n");
    }
}

int cli_get()
{
    int ret = OPTION_NONE;

    fgets(line, sizeof(line), stdin);
    switch (line[0])
    {
    case 'q':
        ret = OPTION_QUIT;
        break;
    case 'e':
        ret = OPTION_RECORD_BACK;
        break;
    case 'w':
        ret = OPTION_MOV_UP;
        break;
    case 's':
        ret = OPTION_MOV_DOWN;
        break;
    case 'a':
        ret = OPTION_MOV_LEFT;
        break;
    case 'd':
        ret = OPTION_MOV_RIGHT;
        break;
    default:
        break;
    }
    // TODO: 输入优化
    return ret;
}