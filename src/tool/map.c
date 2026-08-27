#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "core.h"
#include "ui/cli.h"

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