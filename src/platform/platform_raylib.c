#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "platform.h"

#include "raylib/raylib.h"

#pragma region "window"

static bool should_close = false;

void init_window(int width, int height, const char* title, int flags)
{
    if(is_window_ready())
        return;
    // FLAG_WINDOW_UNDECORATED
    // FLAG_WINDOW_TRANSPARENT
    // FLAG_WINDOW_HIGHDPI
    // FLAG_WINDOW_RESIZABLE
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | flags);
    InitWindow(width, height, title);
    SetExitKey(KEY_NULL);
}

void close_window(void)
{
    if(is_window_ready())
        return;
    CloseWindow();
}

bool is_window_ready(void)
{
    return IsWindowReady();
}

bool window_should_close(void)
{
    return WindowShouldClose() | should_close;
}

void window_set_close(void)
{
    should_close = true;
}

WindowInfo get_window_info(void)
{
    return (WindowInfo) {
        .fps = GetFPS(),
        .width = GetScreenWidth(),
        .height = GetScreenHeight(),
    };
}

#pragma endregion

#pragma region "input"

int get_key_pressed(void)
{
    return GetKeyPressed();
}

#pragma endregion


#pragma region "sys"

void logger(int level, const char *text, ...)
{
    char s[128];
    va_list ap;

    va_start(ap, text);
    vsnprintf(s, sizeof(s), text, ap);
    va_end(ap);
    TraceLog(level, s);
}

void set_log_level(int level)
{
    SetTraceLogLevel(level);
}

#pragma endregion

PlatformServices* get_platform_services(){
    static PlatformServices svc = {
        .get_window_info = get_window_info,
        .window_set_close = window_set_close,

        .malloc = malloc,
        .calloc = calloc,
        .realloc = realloc,
        .free = free,

        .get_key_pressed = get_key_pressed,

        .log = logger,
        .set_log_level = set_log_level,
    };

    return &svc;
}
