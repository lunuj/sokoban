#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include "core.h"
#include "raylib/raylib.h"
#include "tool/map.h"
#include "ui/gui.h"

int main()
{
    Map map;
    load_map_from_txt("res/map/map.txt", &map);
    map.is_recording = true;

    int w = map.w;
    int h = map.h;


    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT);
    InitWindow(w*MAP_BLOCK_SIZE, h*MAP_BLOCK_SIZE, "PUSH BOX");
    SetTargetFPS(60);
    gui_init();

    while (!WindowShouldClose())
    {
        gui_show(&map);
        if(is_win(&map))
        {
            printf("player win!\n");
            break;
        }
        if(update(&map, gui_get()) < 0)
        {
            break;
        }
    }

    CloseWindow();

    return 0;
}