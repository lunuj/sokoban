#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "raylib/raylib.h"

#pragma region "window"
static bool window_should_close = false;

static inline bool impl_init_window(const char* title, int width, int height, int flags)
{
    if (IsWindowReady())
        return false;
    // FLAG_WINDOW_UNDECORATED
    // FLAG_WINDOW_TRANSPARENT
    // FLAG_WINDOW_HIGHDPI
    // FLAG_WINDOW_RESIZABLE
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | flags);
    InitWindow(width, height, title);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    return true;
}

static inline void impl_close_window()
{
    CloseWindow();
}

static inline void impl_set_window_close()
{
    window_should_close = true;
}

static inline bool impl_get_window_state()
{
    return window_should_close | WindowShouldClose();
}

static inline int impl_get_window_width()
{
    return GetScreenWidth();
}

static inline int impl_get_window_height()
{
    return GetScreenHeight();
}

#pragma endregion

#pragma region "input"

static inline int impl_get_key_pressed()
{
    return GetKeyPressed();
}

#pragma endregion

#pragma region "render"

static inline void impl_begin_drawing()
{
    BeginDrawing();
}

static inline void impl_end_drawing()
{
    EndDrawing();
}

static inline void impl_begin_mode_2d(Camera2D camera)
{
    BeginMode2D(camera);
}

static inline void impl_end_mode_2d()
{
    EndMode2D();
}

static inline void impl_clear_background(Color color)
{
    ClearBackground(color);
}

static inline Texture impl_load_texture(const char* filename)
{
    return LoadTexture(filename);
}

static inline void impl_unload_texture(Texture texture)
{
    UnloadTexture(texture);
}

static inline void impl_draw_texture(Texture texture, int posX, int posY, Color tint)
{
    DrawTexture(texture, posX, posY, tint);
}

static inline void impl_draw_texture_pro(
    Texture texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint
)
{
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

static inline void impl_draw_rectangle(int posX, int posY, int width, int height, Color color)
{
    DrawRectangle(posX, posY, width, height, color);
}

static inline void impl_draw_text(const char* text, int posX, int posY, int font_size, Color color)
{
    DrawText(text, posX, posY, font_size, color);
}

#pragma endregion

#pragma region "log"

static inline void impl_log(int level, const char* fmt, ...)
{
    printf("%d %s\n", level, fmt);
}

static inline void impl_set_log_level(int level)
{
    (void)level;
    SetTraceLogLevel(LOG_FATAL);
}

#pragma endregion

#pragma region "system"

static inline void* impl_calloc(size_t count, size_t size)
{
    return calloc(count, size);
}

static inline void impl_free(void* ptr)
{
    return free(ptr);
}

#pragma endregion