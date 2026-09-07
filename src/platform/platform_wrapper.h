// src/platform/platform_wrapper.h
#pragma once

#include "platform_def.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// ============ 平台初始化 ============
void platform_init(PlatformServices *services);
void platform_shutdown(void);
bool platform_is_initialized(void);

// ============ 窗口管理 ============
bool platform_init_window(const char *title, int width, int height, int flags);
void platform_close_window(void);
bool platform_window_should_close(void);
void platform_window_set_close(void);
void platform_set_window_title(const char *title);

// ============ 帧控制 ============
void platform_begin_drawing(void);
void platform_end_drawing(void);
void platform_clear_background(PlatformColor color);
float platform_get_frame_time(void);
void platform_set_target_fps(int fps);
int platform_get_fps(void);

// ============ 输入 ============
bool platform_is_key_pressed(int key);
bool platform_is_key_down(int key);
bool platform_is_key_released(int key);
int platform_get_key_pressed(void);
PlatformPoint platform_get_mouse_position(void);
PlatformPoint platform_get_mouse_delta(void);
float platform_get_mouse_wheel(void);
bool platform_is_mouse_button_pressed(int button);
bool platform_is_mouse_button_down(int button);

// ============ 绘图 ============
void platform_draw_texture(void *texture, int x, int y, PlatformColor color);
void platform_draw_texture_ex(
    void *texture, PlatformPoint pos, float rotation, float scale, PlatformColor color);
void platform_draw_texture_part(void *texture,
                                PlatformRectangle src,
                                PlatformRectangle dst,
                                PlatformColor color);
void platform_draw_rectangle(int x, int y, int w, int h, PlatformColor color);
void platform_draw_rectangle_lines(int x, int y, int w, int h, PlatformColor color);
void platform_draw_text(
    const char *text, int x, int y, int font_size, PlatformColor color);
void platform_draw_text_ex(void *font,
                           const char *text,
                           PlatformPoint pos,
                           float size,
                           float spacing,
                           PlatformColor color);

// ============ 资源加载 ============
void *platform_load_texture(const char *path);
void platform_unload_texture(void *texture);
void *platform_load_font(const char *path, int size);
void platform_unload_font(void *font);
void *platform_load_sound(const char *path);
void platform_unload_sound(void *sound);

// ============ 音频 ============
void platform_init_audio(void);
void platform_close_audio(void);
void platform_play_sound(void *sound);
void platform_set_master_volume(float volume);
float platform_get_master_volume(void);

// ============ 文件系统 ============
void *platform_load_file(const char *path, size_t *out_size);
bool platform_save_file(const char *path, const void *data, size_t size);
bool platform_file_exists(const char *path);

// ============ 内存管理 ============
void *platform_malloc(size_t size);
void *platform_calloc(size_t count, size_t size);
void *platform_realloc(void *ptr, size_t size);
void platform_free(void *ptr);
char *platform_strdup(const char *str);

// ============ 日志 ============
void platform_log(int level, const char *fmt, ...);
void platform_log_v(int level, const char *fmt, va_list args);

// ============ 时间/随机 ============
double platform_get_time(void);
void platform_set_random_seed(uint64_t seed);
int platform_random_int(int min, int max);
float platform_random_float(float min, float max);

// ============ 热重载支持 ============
void *platform_get_services_for_reload(void);
void platform_restore_services(void *state);