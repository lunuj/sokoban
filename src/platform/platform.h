#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct WindowInfo {
    int width;
    int height;
    int fps;
    int flag;
} WindowInfo;

void init_window(int width, int height, const char * title, int flags);
void close_window(void);
bool is_window_ready(void);
bool window_should_close(void);
void window_set_close(void);
WindowInfo get_window_info(void);

int get_key_pressed(void);

void logger(int level, const char *text, ...);
void set_log_level(int level);


#pragma region "plugin"

typedef struct PlatformServices {
    WindowInfo (*get_window_info)(void);
    void (*window_set_close)(void);

    void* (*malloc)(size_t size);
    void* (*calloc)(size_t count, size_t size);
    void* (*realloc)(void* ptr, size_t size);
    void (*free)(void* ptr);
    
    int (*get_key_pressed)(void);

    void (*log)(int level, const char* fmt, ...);
    void (*set_log_level)(int level);
    
    double (*get_time)(void);
    int (*random_int)(int min, int max);
    float (*random_float)(float min, float max);
    
} PlatformServices;

PlatformServices *get_platform_services();

#pragma endregion
