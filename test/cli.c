#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include "core.h"

extern char map[MAP_HEIGHT][MAP_WIDTH];

char c = 0;
char line[128];

void show()
{
    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void getinput()
{
    fgets(line, sizeof(line), stdin);

    c = line[0];

    for(int j = 0; j < MAP_WIDTH+1; j++)
    {
        printf("\033[A");
    }
    printf("\r");
}


int main()
{
    while(!is_win())
    {
        show();
        getinput();
        update(c);
    }
    printf("player win!\n");

    return 0;
}