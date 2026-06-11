#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include "core.h"
#include "raylib/raylib.h"

#define MAP_BLOCK_SIZE 13
extern char map[MAP_HEIGHT][MAP_WIDTH];

static char last = 0;
static int count = 0;

void show()
{
    Color color;
    BeginDrawing();
    ClearBackground(BLANK);
    
    for(int i = 0; i < MAP_HEIGHT*MAP_WIDTH; i++)
    {
        switch(*((char *)map + i))
        {
            case BOLCK:
                color = BLACK;
                break;
            case SPACE:
                color = BLANK;
                break;
            case PLAYER:
                color = BROWN;
                break;
            case TARGET:
                color = BLUE;
                break;
            case BOX:
                color = RED;
                break;
            case PLAYER_ON_TARGET:
                color = SKYBLUE;
                break;
            case BOX_ON_TARGET:
                color = GREEN;
                break;
            default:
                break;
        }
        DrawRectangle((i%MAP_WIDTH)*MAP_BLOCK_SIZE, (i/MAP_WIDTH)*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, color); // x, y, 宽, 高, 颜色
    }
    EndDrawing();
}

char getinput()
{
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return 'd';
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return 'a';
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))  return 'w';
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) return 's';
    if (IsKeyPressed(KEY_E)) return 'e';
    if (IsKeyPressed(KEY_Q)) return 'q';
    return ' ';
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT);
    InitWindow(MAP_WIDTH*MAP_BLOCK_SIZE, MAP_HEIGHT*MAP_BLOCK_SIZE, "Hello Raylib");
    while (!WindowShouldClose())
    {
        show();
        if(is_win())
        {
            printf("player win!\n");
            exit(0);
        }
        
        update(getinput());
    }
    CloseWindow();

    return 0;
}