#pragma once

/* 本文件由 gen_from_impl_h.py 自动生成, 请勿手动修改 */

#include <stdio.h>
#include <stdlib.h>
#include "raylib/raylib.h"

bool platform_init_window(const char* title, int width, int height, int flags);
void platform_close_window(void);
void platform_set_window_close(void);
bool platform_get_window_state(void);
int platform_get_window_width(void);
int platform_get_window_height(void);
int platform_get_key_pressed(void);
void platform_begin_drawing(void);
void platform_end_drawing(void);
void platform_begin_mode_2d(Camera2D camera);
void platform_end_mode_2d(void);
void platform_clear_background(Color color);
Texture platform_load_texture(const char* filename);
void platform_unload_texture(Texture texture);
void platform_draw_texture(Texture texture, int posX, int posY, Color tint);
void platform_draw_texture_pro(Texture texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
void platform_draw_rectangle(int posX, int posY, int width, int height, Color color);
void platform_draw_text(const char* text, int posX, int posY, int font_size, Color color);
void platform_log(int level, const char* fmt, ...);
void platform_set_log_level(int level);
void* platform_calloc(size_t count, size_t size);
void platform_free(void* ptr);

typedef struct PlatformApi PlatformApi;
PlatformApi* platform_get_api(void);
