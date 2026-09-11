#pragma once

/* 本文件由 gen_from_impl_h.py 自动生成, 请勿手动修改 */

#include "platform_def.h"

typedef struct PlatformApi PlatformApi;

bool plugin_init_window(const char* title, int width, int height, int flags);

void plugin_close_window(void);

void plugin_set_window_close(void);

bool plugin_get_window_state(void);

int plugin_get_window_width(void);

int plugin_get_window_height(void);

int plugin_get_key_pressed(void);

void plugin_begin_drawing(void);

void plugin_end_drawing(void);

void plugin_clear_background(Color color);

Texture plugin_load_texture(const char* filename);

void plugin_unload_texture(Texture texture);

void plugin_draw_texture(Texture texture, int posX, int posY, Color tint);

void plugin_draw_texture_pro(Texture texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);

void plugin_draw_rectangle(int posX, int posY, int width, int height, Color color);

void plugin_draw_text(const char* text, int posX, int posY, int font_size, Color color);

void plugin_log(int level, const char* fmt, ...);

void plugin_set_log_level(int level);

void* plugin_calloc(size_t count, size_t size);

void plugin_free(void* ptr);

void plugin_set_api(PlatformApi *api);
