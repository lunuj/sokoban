#include <stdio.h>
#include <stdlib.h>

#include "sokoban/core/core.h"
#include "sokoban/core/map.h"
#include "sokoban/core/records.h"
#include "sokoban/tool/xsb.h"

Map map;
Records records;
char c = 0;     /**< 临时字符变量 */
char line[128]; /**< 输入行缓冲区 */

void cli_show(Map* map)
{
    for (int i = 0; i < map->h; i++)
    {
        for (int j = 0; j < map->w; j++)
        {
            printf("%c", map_to_xsb(map->map[i * map->w + j]));
        }
        printf("\n");
    }
}

int cli_get()
{
    int ret = CMD_NONE;

    fgets(line, sizeof(line), stdin);
    switch (line[0])
    {
    case 'q':
        ret = CMD_BACK;
        exit(0);
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

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        load_map_from_xsb("res/map/map01.xsb", &map);
    }
    else
    {
        if (load_map_from_xsb(argv[1], &map) < 0)
        {
            return 1;
        }
    }

    while (!core_is_win(&map))
    {
        cli_show(&map);
        core_update(&map, &records, cli_get());
    }

    printf("player win!\n");
    return 0;
}