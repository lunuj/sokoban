
#include "raylib/raylib.h"
#include "ui/scene.h"

int main()
{
    const int screenWidth = 600;
    const int screenHeight = 400;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "handmake");
    SetTargetFPS(60);

    scene_init();
    while (!scene_is_end())
    {
        scene_draw();
        scene_update(scene_key_map(GetKeyPressed()));
    }
    scene_exit();

    CloseWindow();

    return 0;
}
