#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include <stdint.h> 
#include "core.h"
#include "ui/gui.h"
#include "raylib/raylib.h"
#include "anim.h"

static double last = 0;
Texture2D texture[MAX];

int speed_x = 1;
int speed_y = 1;
int anmi_t = 1;

Color get_block_color(int block)
{
    uint32_t color = 0;

    if((block & MAP_BLOCK) == MAP_BLOCK)
        color |= ColorToInt(BLACK);
    if((block & MAP_SPACE) == MAP_SPACE)
        color |= ColorToInt(BLANK);
    if((block & MAP_PLAYER) == MAP_PLAYER)
        color |= ColorToInt(BROWN);
    if((block & MAP_TARGET) == MAP_TARGET)
        color |= ColorToInt(BLUE);
    if((block & MAP_BOX) == MAP_BOX)
        color |= ColorToInt(RED);
    
    return GetColor(color);
}


void mov_block(Map *map, Record* record)
{
    Color color;
    int w = map->w;
    int h = map->h;

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

    
    color = get_block_color(*(map->map + record->pos_t));
    DrawRectangle((record->pos_t%w)*MAP_BLOCK_SIZE, (record->pos_t/w)*MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, MAP_BLOCK_SIZE, color); // x, y, 宽, 高, 颜色
    color = get_block_color(*(map->map + record->pos_t));
    DrawRectangle(
        (record->pos_f%map->w)+speed_x*anmi_t,
        (record->pos_f/map->w)+speed_y*anmi_t,
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

void gui_show_back(int block, int x, int y)
{
    if((block & MAP_SPACE) == MAP_SPACE)
        DrawTexture(texture[SPACE],x,y,WHITE);
    if((block & MAP_TARGET) == MAP_TARGET)
        DrawTexture(texture[TARGET],x,y,WHITE);
}

void gui_show_fornt(int block, int x, int y)
{
    if((block & MAP_BLOCK) == MAP_BLOCK)
        DrawTexture(texture[BLOCK],x,y,WHITE);
    if((block & MAP_PLAYER) == MAP_PLAYER)
        DrawTexture(texture[PLAYER],x,y,WHITE);
    if((block & MAP_BOX) == MAP_BOX)
        DrawTexture(texture[BOX],x,y,WHITE);
}

static Animation animation = {0};
static Anim anim;
static int count = 0;

void gui_show(Map *map)
{
    Color color;
    
    int w = map->w;
    int h = map->h;

    BeginDrawing();
    ClearBackground(BLANK);
    
    static Vector2 pos;

    int t = map->map[map->records[count].pos_t];
    int f = map->map[map->records[count].pos_f];
    int pos_x = (map->records[count].pos_f%w)*MAP_BLOCK_SIZE;
    int pos_y = (map->records[count].pos_f/w)*MAP_BLOCK_SIZE;

    for(int i = 0; i < w*h; i++)
    {
        int pos_x = (i%w)*MAP_BLOCK_SIZE;
        int pos_y = (i/w)*MAP_BLOCK_SIZE;
        int block = *(map->map + i);
        gui_show_back(block, pos_x, pos_y);
        if(i == map->records[count].pos_t || i == map->records[count].pos_f)
        if(count < map->record_count)
            continue;
        gui_show_fornt(block, pos_x, pos_y);
    }

    if(count < map->record_count)
    {
        if(animation.elapsed == 0)
        {
            AnimValue p1_to = {
                .type = ANIMATION_POSITION,
                .from.position = {(map->records[count].pos_f%w)*MAP_BLOCK_SIZE, (map->records[count].pos_f/w)*MAP_BLOCK_SIZE},
                .to.position = {(map->records[count].pos_t%w)*MAP_BLOCK_SIZE, (map->records[count].pos_t/w)*MAP_BLOCK_SIZE},
            };
            animation_init(&animation, 1, p1_to, &pos);
            anim_init(&anim, ANIM_ANIMATION, &animation);
        }
        anim_update(&anim, 0.1);
        gui_show_fornt(t, pos.x, pos.y);
        if(animation.elapsed >= animation.duration)
        {
            count++;
            anim_reset(&anim);
        }
    }

    EndDrawing();
}

int gui_init()
{
    Image box = LoadImage("res/img/box.png");
    Image player = LoadImage("res/img/player.png");
    Image space = LoadImage("res/img/space.png");
    Image target = LoadImage("res/img/target.png");
    Image block = LoadImage("res/img/block.png");

    texture[BOX] = LoadTextureFromImage(box);
    texture[PLAYER] = LoadTextureFromImage(player);
    texture[SPACE] = LoadTextureFromImage(space);
    texture[TARGET] = LoadTextureFromImage(target);
    texture[BLOCK] = LoadTextureFromImage(block);

    UnloadImage(box);
    UnloadImage(player);
    UnloadImage(space);
    UnloadImage(target);
    UnloadImage(block);

    return 0;
}

int gui_get()
{
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return OPTION_MOV_RIGHT;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return OPTION_MOV_LEFT;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))  return OPTION_MOV_UP;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) return OPTION_MOV_DOWN;
    if (IsKeyPressed(KEY_E)) return OPTION_RECORD_BACK;
    if (IsKeyPressed(KEY_Q)) return OPTION_QUIT;
    return OPTION_NONE;
}