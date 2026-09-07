// src/platform/platform_wrapper.c
#include "platform_wrapper.h"
#include "platform_services.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ============ 内部状态 ============
typedef struct PlatformServices PlatformServices;

static PlatformServices* svc = NULL;
static bool initialized = false;
static int log_level = 0;

// ============ 日志 ============
static const char* log_level_str[] = {
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

void platform_log(int level, const char* fmt, ...) {
    if (level < log_level) return;
    
    va_list args;
    va_start(args, fmt);
    platform_log_v(level, fmt, args);
    va_end(args);
}

void platform_log_v(int level, const char* fmt, va_list args) {
    if (level < log_level) return;
    
    printf("[%s] ", log_level_str[level]);
    vprintf(fmt, args);
    printf("\n");
}

// ============ 初始化 ============

void platform_init(PlatformServices* services) {
    svc = services;
    initialized = true;
    platform_log(PLATFORM_LOG_INFO, "Platform wrapper initialized");
}

void platform_shutdown(void) {
    initialized = false;
    svc = NULL;
    platform_log(PLATFORM_LOG_INFO, "Platform wrapper shutdown");
}

bool platform_is_initialized(void) {
    return initialized && svc != NULL;
}

// ============ 辅助宏 ============
#define CHECK_INIT() \
    do { \
        if (!platform_is_initialized()) { \
            platform_log(PLATFORM_LOG_ERROR, "Platform not initialized"); \
            return; \
        } \
    } while(0)

#define CHECK_INIT_RET(ret) \
    do { \
        if (!platform_is_initialized()) { \
            platform_log(PLATFORM_LOG_ERROR, "Platform not initialized"); \
            return ret; \
        } \
    } while(0)

#define CHECK_INIT_RET_DEFAULT(ret) \
    do { \
        if (!platform_is_initialized()) { \
            platform_log(PLATFORM_LOG_ERROR, "Platform not initialized"); \
            return (ret); \
        } \
    } while(0)

// ============ 窗口管理 ============

bool platform_init_window(const char* title, int width, int height, int flags) {
    CHECK_INIT_RET_DEFAULT(false);
    return svc->init_window(title, width, height, flags);
}

void platform_close_window(void) {
    CHECK_INIT();
    svc->close_window();
}

bool platform_window_should_close(void) {
    CHECK_INIT_RET_DEFAULT(true);
    return svc->window_should_close();
}

void platform_window_set_close(void) {
    CHECK_INIT();
    svc->window_set_close();
}

void platform_set_window_title(const char* title) {
    CHECK_INIT();
    svc->set_window_title(title);
}

// ============ 帧控制 ============

void platform_begin_drawing(void) {
    CHECK_INIT();
    svc->begin_drawing();
}

void platform_end_drawing(void) {
    CHECK_INIT();
    svc->end_drawing();
}

void platform_clear_background(PlatformColor color) {
    CHECK_INIT();
    svc->clear_background(color);
}

float platform_get_frame_time(void) {
    CHECK_INIT_RET_DEFAULT(0.0f);
    return svc->get_frame_time();
}

void platform_set_target_fps(int fps) {
    CHECK_INIT();
    svc->set_target_fps(fps);
}

int platform_get_fps(void) {
    CHECK_INIT_RET_DEFAULT(0);
    return svc->get_fps();
}

// ============ 输入 ============

bool platform_is_key_pressed(int key) {
    CHECK_INIT_RET_DEFAULT(false);
    return svc->is_key_pressed(key);
}

bool platform_is_key_down(int key) {
    CHECK_INIT_RET_DEFAULT(false);
    return svc->is_key_down(key);
}

bool platform_is_key_released(int key) {
    CHECK_INIT_RET_DEFAULT(false);
    return svc->is_key_released(key);
}

int platform_get_key_pressed(void) {
    CHECK_INIT_RET_DEFAULT(0);
    return svc->get_key_pressed();
}

PlatformPoint platform_get_mouse_position(void) {
    PlatformPoint default_pos = {0, 0};
    CHECK_INIT_RET_DEFAULT(default_pos);
    return svc->get_mouse_position();
}

PlatformPoint platform_get_mouse_delta(void) {
    PlatformPoint default_delta = {0, 0};
    CHECK_INIT_RET_DEFAULT(default_delta);
    return svc->get_mouse_delta();
}

float platform_get_mouse_wheel(void) {
    CHECK_INIT_RET_DEFAULT(0.0f);
    return svc->get_mouse_wheel();
}

bool platform_is_mouse_button_pressed(int button) {
    CHECK_INIT_RET_DEFAULT(false);
    return svc->is_mouse_button_pressed(button);
}

bool platform_is_mouse_button_down(int button) {
    CHECK_INIT_RET_DEFAULT(false);
    return svc->is_mouse_button_down(button);
}

// ============ 绘图 ============

void platform_draw_texture(void* texture, int x, int y, PlatformColor color) {
    CHECK_INIT();
    if (!texture) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to draw null texture");
        return;
    }
    svc->draw_texture(texture, x, y, color);
}

void platform_draw_texture_ex(void* texture, PlatformPoint pos, float rotation, float scale, PlatformColor color) {
    CHECK_INIT();
    if (!texture) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to draw null texture");
        return;
    }
    svc->draw_texture_ex(texture, pos, rotation, scale, color);
}

void platform_draw_rectangle(int x, int y, int w, int h, PlatformColor color) {
    CHECK_INIT();
    svc->draw_rectangle(x, y, w, h, color);
}

void platform_draw_rectangle_lines(int x, int y, int w, int h, PlatformColor color) {
    CHECK_INIT();
    svc->draw_rectangle_lines(x, y, w, h, color);
}

void platform_draw_text(const char* text, int x, int y, int font_size, PlatformColor color) {
    CHECK_INIT();
    if (!text) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to draw null text");
        return;
    }
    svc->draw_text(text, x, y, font_size, color);
}

void platform_draw_text_ex(void* font, const char* text, PlatformPoint pos, float size, float spacing, PlatformColor color) {
    CHECK_INIT();
    if (!font) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to draw with null font");
        return;
    }
    if (!text) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to draw null text");
        return;
    }
    svc->draw_text_ex(font, text, pos, size, spacing, color);
}

// ============ 资源加载 ============

void* platform_load_texture(const char* path) {
    CHECK_INIT_RET_DEFAULT(NULL);
    if (!path) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to load texture with null path");
        return NULL;
    }
    void* result = svc->load_texture(path);
    if (!result) {
        platform_log(PLATFORM_LOG_WARN, "Failed to load texture: %s", path);
    }
    return result;
}

void platform_unload_texture(void* texture) {
    CHECK_INIT();
    if (!texture) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to unload null texture");
        return;
    }
    svc->unload_texture(texture);
}

void* platform_load_font(const char* path, int size) {
    CHECK_INIT_RET_DEFAULT(NULL);
    if (!path) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to load font with null path");
        return NULL;
    }
    void* result = svc->load_font(path, size);
    if (!result) {
        platform_log(PLATFORM_LOG_WARN, "Failed to load font: %s", path);
    }
    return result;
}

void platform_unload_font(void* font) {
    CHECK_INIT();
    if (!font) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to unload null font");
        return;
    }
    svc->unload_font(font);
}

void* platform_load_sound(const char* path) {
    CHECK_INIT_RET_DEFAULT(NULL);
    if (!path) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to load sound with null path");
        return NULL;
    }
    void* result = svc->load_sound(path);
    if (!result) {
        platform_log(PLATFORM_LOG_WARN, "Failed to load sound: %s", path);
    }
    return result;
}

void platform_unload_sound(void* sound) {
    CHECK_INIT();
    if (!sound) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to unload null sound");
        return;
    }
    svc->unload_sound(sound);
}

// ============ 音频 ============

void platform_init_audio(void) {
    CHECK_INIT();
    svc->init_audio();
    platform_log(PLATFORM_LOG_INFO, "Audio initialized");
}

void platform_close_audio(void) {
    CHECK_INIT();
    svc->close_audio();
    platform_log(PLATFORM_LOG_INFO, "Audio closed");
}

void platform_play_sound(void* sound) {
    CHECK_INIT();
    if (!sound) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to play null sound");
        return;
    }
    svc->play_sound(sound);
}

void platform_set_master_volume(float volume) {
    CHECK_INIT();
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    svc->set_master_volume(volume);
}

float platform_get_master_volume(void) {
    CHECK_INIT_RET_DEFAULT(1.0f);
    return svc->get_master_volume();
}

// ============ 文件系统 ============

void* platform_load_file(const char* path, size_t* out_size) {
    CHECK_INIT_RET_DEFAULT(NULL);
    if (!path) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to load file with null path");
        return NULL;
    }
    return svc->load_file(path, out_size);
}

bool platform_save_file(const char* path, const void* data, size_t size) {
    CHECK_INIT_RET_DEFAULT(false);
    if (!path || !data) {
        platform_log(PLATFORM_LOG_WARN, "Attempt to save file with null path or data");
        return false;
    }
    return svc->save_file(path, data, size);
}

bool platform_file_exists(const char* path) {
    CHECK_INIT_RET_DEFAULT(false);
    if (!path) return false;
    return svc->file_exists(path);
}

// ============ 内存管理 ============

void* platform_malloc(size_t size) {
    CHECK_INIT_RET_DEFAULT(malloc(size));
    return svc->malloc ? svc->malloc(size) : malloc(size);
}

void* platform_calloc(size_t count, size_t size) {
    CHECK_INIT_RET_DEFAULT(calloc(count, size));
    return svc->calloc ? svc->calloc(count, size) : calloc(count, size);
}

void* platform_realloc(void* ptr, size_t size) {
    CHECK_INIT_RET_DEFAULT(realloc(ptr, size));
    return svc->realloc ? svc->realloc(ptr, size) : realloc(ptr, size);
}

void platform_free(void* ptr) {
    if (initialized && svc && svc->free) {
        svc->free(ptr);
    } else {
        free(ptr);
    }
}

char* platform_strdup(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char* result = platform_malloc(len);
    if (result) {
        memcpy(result, str, len);
    }
    return result;
}

// ============ 时间/随机 ============

double platform_get_time(void) {
    CHECK_INIT_RET_DEFAULT(0.0);
    return svc->get_time();
}

void platform_set_random_seed(uint64_t seed) {
    CHECK_INIT();
    if (svc->set_random_seed) {
        svc->set_random_seed(seed);
    }
}

int platform_random_int(int min, int max) {
    CHECK_INIT_RET_DEFAULT(0);
    return svc->random_int ? svc->random_int(min, max) : 0;
}

float platform_random_float(float min, float max) {
    CHECK_INIT_RET_DEFAULT(0.0f);
    return svc->random_float ? svc->random_float(min, max) : 0.0f;
}

// ============ 热重载支持 ============

void* platform_get_services_for_reload(void) {
    return svc;
}

void platform_restore_services(void* state) {
    svc = (PlatformServices*)state;
    initialized = true;
    platform_log(PLATFORM_LOG_INFO, "Platform services restored for reload");
}