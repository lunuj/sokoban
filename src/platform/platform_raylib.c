/**
 * @file platform.c
 * @brief 平台服务实现模块
 * @details 基于raylib库提供窗口管理、输入处理和系统日志等平台服务
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "platform.h"

#include "raylib/raylib.h"

#pragma region "window"

/** @brief 窗口关闭标志 */
static bool should_close = false;

/**
 * @brief 初始化窗口
 * @param width 窗口宽度
 * @param height 窗口高度
 * @param title 窗口标题
 * @param flags 窗口配置标志
 * @note 如果窗口已就绪则直接返回
 */
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

/**
 * @brief 关闭窗口
 * @note 如果窗口未就绪则直接返回
 */
void close_window(void)
{
    if(is_window_ready())
        return;
    CloseWindow();
}

/**
 * @brief 检查窗口是否就绪
 * @return 窗口已就绪返回true，否则返回false
 */
bool is_window_ready(void)
{
    return IsWindowReady();
}

/**
 * @brief 检查窗口是否应该关闭
 * @return 窗口应关闭返回true，否则返回false
 */
bool window_should_close(void)
{
    return WindowShouldClose() | should_close;
}

/**
 * @brief 设置窗口关闭标志
 */
void window_set_close(void)
{
    should_close = true;
}

/**
 * @brief 获取窗口信息
 * @return 包含FPS、宽度和高度的WindowInfo结构体
 */
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

/**
 * @brief 获取按下的按键
 * @return 按键码，无按键时返回0
 */
int get_key_pressed(void)
{
    return GetKeyPressed();
}

#pragma endregion


#pragma region "sys"

/**
 * @brief 日志记录函数
 * @param level 日志级别
 * @param text 格式化字符串
 * @param ... 可变参数
 */
void logger(int level, const char *text, ...)
{
    char s[128];
    va_list ap;

    va_start(ap, text);
    vsnprintf(s, sizeof(s), text, ap);
    va_end(ap);
    TraceLog(level, s);
}

/**
 * @brief 设置日志级别
 * @param level 日志级别
 */
void set_log_level(int level)
{
    SetTraceLogLevel(level);
}

#pragma endregion

/**
 * @brief 获取平台服务接口指针
 * @return PlatformServices结构体指针（单例）
 * @details 提供窗口、内存、输入和日志等服务的函数指针
 */
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