#pragma once

/* 本文件由 gen_from_impl_h.py 自动生成, 请勿手动修改 */


typedef struct PlatformApi {
    bool (*init_window)(const char* title, int width, int height, int flags);
    void (*close_window)(void);
    void (*set_window_close)(void);
    bool (*get_window_state)(void);
    int (*get_window_width)(void);
    int (*get_window_height)(void);
    int (*get_key_pressed)(void);
    void (*begin_drawing)(void);
    void (*end_drawing)(void);
    void (*clear_background)(Color color);
    Texture (*load_texture)(const char* filename);
    void (*unload_texture)(Texture texture);
    void (*draw_texture)(Texture texture, int posX, int posY, Color tint);
    void (*draw_texture_pro)(Texture texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
    void (*draw_rectangle)(int posX, int posY, int width, int height, Color color);
    void (*draw_text)(const char* text, int posX, int posY, int font_size, Color color);
    void (*log)(int level, const char* fmt, ...);
    void (*set_log_level)(int level);
    void* (*calloc)(size_t count, size_t size);
    void (*free)(void* ptr);
} PlatformApi;
