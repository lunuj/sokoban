#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include "core.h"
#include "raylib/raylib.h"

#define MAP_BLOCK_SIZE 13

#define BLOCK_POS_X(pos)  (pos%MAP_WIDTH)*MAP_BLOCK_SIZE
#define BLOCK_POS_Y(pos)  (pos/MAP_WIDTH)*MAP_BLOCK_SIZE

extern char map[MAP_HEIGHT][MAP_WIDTH];
extern int record_count;
extern struct record records[1024];

static double last = 0;
static int count = 0;

int speed_x = 1;
int speed_y = 1;
int anmi_t = 1;

Color get_block_color(int block)
{
    Color color;

    switch(block)
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
            color = BLANK;
            break;
    }
    return color;
}


void mov_block(struct record* record)
{
    Color color;

    switch(record->direct)
    {
        case 'a':
            speed_x = -1;
            speed_y = 0;
            break;
        case 's':
            speed_x = 0;
            speed_y = 1;
            break;
        case 'd':
            speed_x = 1;
            speed_y = 0;
            break;
        case 'w':
            speed_x = 0;
            speed_y = -1;
            break;
        default:
            return;
    }

    color = get_block_color(*((char *)map + record->pos_t));
    DrawRectangle((record->pos_t%MAP_WIDTH)*MAP_BLOCK_SIZE, (record->pos_t/MAP_WIDTH)*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, color); // x, y, 宽, 高, 颜色
    color = get_block_color(*((char *)map + record->pos_t));
    DrawRectangle(
        BLOCK_POS_X(record->pos_f)+speed_x*anmi_t,
        BLOCK_POS_Y(record->pos_f)+speed_y*anmi_t,
        MAP_BLOCK_SIZE,
        MAP_BLOCK_SIZE,
        color
    );
    double time_now = GetTime();
    if(time_now - last > 0.01)
    {
        anmi_t++;
        last = time_now;
    }
}

void show()
{
    Color color;
    
    BeginDrawing();
    ClearBackground(BLANK);

    for(int i = 0; i < MAP_HEIGHT*MAP_WIDTH; i++)
    {

        if(count < record_count)
        {
            if(records[count].pos_t == i)
                continue;
            if(records[count].pos_f == i)
            {
                mov_block(&records[count]);

                if(anmi_t > MAP_BLOCK_SIZE)
                {
                    anmi_t = 1;
                    count++;
                }
                continue;
            }
        }
        else
        {
            count = record_count;
            
        }

        color = get_block_color(*((char *)map + i));
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
        if(count == record_count)
            update(getinput());
    }
    CloseWindow();

    return 0;
}