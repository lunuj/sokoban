#pragma once
#include "platform_def.h"

/**
 * @brief 平台服务接口结构体
 * @details 提供插件所需的各类平台服务函数指针
 */
typedef struct PlatformServices {
    /* ---- 窗口管理 ---- */
    bool (*init_window)(const char* title, int width, int height, int flags);
    void (*close_window)(void);
    bool (*window_should_close)(void);
    void (*window_set_close)(void);
    PlatformWindowInfo (*get_window_info)(void);
    void (*set_window_title)(const char* title);
    void (*set_window_size)(int width, int height);

    /* ---- 帧控制 ---- */
    void (*begin_drawing)(void);
    void (*end_drawing)(void);
    void (*clear_background)(PlatformColor color);
    float (*get_frame_time)(void);
    void (*set_target_fps)(int fps);
    int (*get_fps)(void);

    /* ---- 输入 ---- */
    bool (*is_key_pressed)(int key);
    bool (*is_key_down)(int key);
    bool (*is_key_released)(int key);
    int (*get_key_pressed)(void);
    int (*key_to_plugin)(int platform_key);
    int (*key_to_platform)(int plugin_key);
    int (*get_modifiers)(void);

    bool (*is_mouse_button_pressed)(int button);
    bool (*is_mouse_button_down)(int button);
    PlatformPoint (*get_mouse_position)(void);
    PlatformPoint (*get_mouse_delta)(void);
    float (*get_mouse_wheel)(void);

    /* ---- 图形 ---- */
    void (*draw_texture)(void* texture, int x, int y, PlatformColor color);
    void (*draw_texture_ex)(void* texture, PlatformPoint pos, float rotation, float scale, PlatformColor color);
    void (*draw_rectangle)(int x, int y, int width, int height, PlatformColor color);
    void (*draw_rectangle_lines)(int x, int y, int width, int height, PlatformColor color);
    void (*draw_text)(const char* text, int x, int y, int font_size, PlatformColor color);
    void (*draw_text_ex)(void* font, const char* text, PlatformPoint pos, float font_size, float spacing, PlatformColor color);
    PlatformSize (*measure_text)(void* font, const char* text, float font_size, float spacing);

    /* ---- 资源 ---- */
    void* (*load_texture)(const char* path);
    void (*unload_texture)(void* texture);
    void* (*load_font)(const char* path, int size);
    void (*unload_font)(void* font);
    void* (*load_sound)(const char* path);
    void (*unload_sound)(void* sound);
    void (*play_sound)(void* sound);
    void (*stop_sound)(void* sound);

    /* ---- 音频 ---- */
    void (*init_audio)(void);
    void (*close_audio)(void);
    void (*set_master_volume)(float volume);
    float (*get_master_volume)(void);

    /* ---- 内存 ---- */
    void* (*malloc)(size_t size);
    void* (*calloc)(size_t count, size_t size);
    void* (*realloc)(void* ptr, size_t size);
    void (*free)(void* ptr);
    char* (*strdup)(const char* str);

    /* ---- 日志 ---- */
    void (*log)(PlatformLogLevel level, const char* fmt, ...);
    void (*log_v)(PlatformLogLevel level, const char* fmt, va_list args);
    void (*set_log_level)(int level);

    /* ---- 时间/随机 ---- */
    double (*get_time)(void);
    void (*set_random_seed)(uint64_t seed);
    int (*random_int)(int min, int max);
    float (*random_float)(float min, float max);

    /* ---- 文件 ---- */
    uint8_t* (*load_file)(const char* path, size_t* out_size);
    bool (*save_file)(const char* path, const uint8_t* data, size_t size);
    bool (*file_exists)(const char* path);
} PlatformServices;