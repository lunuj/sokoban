#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "core.h"

struct record records[1024];
int record_count = 0;

char map[MAP_HEIGHT][MAP_WIDTH] = {
    "##########",
    "#A       #", 
    "#  ##X   #",
    "#  ##    #",
    "#        #",
    "#   ##   #",
    "#   ##  _#",
    "#   X    #",
    "#   _    #",
    "##########"
};

int find_player()
{
    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            if(map[i][j] == PLAYER || map[i][j] == PLAYER_ON_TARGET)
            {
                return i * MAP_WIDTH + j;   
            }
        }
    }
    return -1;
}


static bool is_recording = true;

void records_add(int pos_f, int direct, int step, int pos_t)
{
    if (is_recording)
    {
        records[record_count].pos_f = pos_f;
        records[record_count].direct = direct;
        records[record_count].step = step;
        records[record_count].pos_t = pos_t;
        record_count++;
    }
}

void mov(int pos_f, int direct, int step)
{
    int pos_t = pos_f;

    switch (direct)
    {   
        case 'w':
            pos_t = pos_f - MAP_WIDTH * step;
            break;
        case 's':
            pos_t = pos_f + MAP_WIDTH * step;
            break;
        case 'a':
            pos_t = pos_f - step;
            break;
        case 'd':
            pos_t = pos_f + step;
            break;
    default:
        break;
    }
    if(pos_t < 0 || pos_t >= MAP_WIDTH * MAP_HEIGHT)
    {
        return;
    }

    char *f = (char *)map + pos_f;
    char *t = (char *)map + pos_t;

    switch (*t)
    {
    case BOLCK:
        return;
    case SPACE:
        if(*f==PLAYER_ON_TARGET)
        {
            *f = TARGET;
            *t = PLAYER;
        }
        else if(*f==BOX_ON_TARGET)
        {
            *f = TARGET;
            *t = BOX;
        }
        else
        {
            *t ^= *f;
            *f ^= *t;
            *t ^= *f;
        }
        break;
    case TARGET:
        if(*f == PLAYER)
        {
            *f = SPACE;
            *t = PLAYER_ON_TARGET;
        }
        else if(*f == BOX)
        {
            *f = SPACE;
            *t = BOX_ON_TARGET;
        }
        break;
    case BOX:
        if(*f!=BOX && *f!=BOX_ON_TARGET)
        {
            mov(pos_t, direct, 1);
        }
        return;
    case BOX_ON_TARGET:
        if(*f!=BOX && *f!=BOX_ON_TARGET)
        {
            mov(pos_t, direct, 1);
        }
        return;
    default:
        break;
    }

    records_add(pos_f, direct, step, pos_t);
}


void records_undo()
{
    if(record_count > 0)
    {
        record_count--;
        is_recording = false;
        int direct = records[record_count].direct;
        int step = records[record_count].step;
        int pos_t = records[record_count].pos_t;

        switch (direct)
        {
        case 'w':
            mov(pos_t, 's', step);
            break;
        case 's':
            mov(pos_t, 'w', step);
            break;
        case 'a':
            mov(pos_t, 'd', step);
            break;
        case 'd':
            mov(pos_t, 'a', step);
            break;
        default:
            break;
        }
        is_recording = true;
    }
}


void update(char c)
{
    int pos = find_player();

    switch (c)
    {
    case 'w':
        mov(pos, 'w', 1);
        break;
    case 's':
        mov(pos, 's', 1);
        break;
    case 'a':
        mov(pos, 'a', 1);
        break;
    case 'd':
        mov(pos, 'd', 1);
        break;
    case 'q':
        exit(0);
        break;
    case 'e':
        records_undo();
        break;        
    default:
        break;
    }
}

bool is_win()
{
    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            if(map[i][j] == BOX)
            {
                return false;   
            }
        }
    }
    return true;
}

