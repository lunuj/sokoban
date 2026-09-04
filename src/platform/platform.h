/**
 * @file platform.h
 * @brief 平台服务模块头文件
 * @details 提供窗口管理、输入处理、日志记录和平台服务接口
 */

#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief 窗口信息结构体
 * @details 包含窗口的尺寸、帧率和标志等信息
 */
typedef struct WindowInfo {
    int width;      /**< 窗口宽度 */
    int height;     /**< 窗口高度 */
    int fps;        /**< 当前帧率 */
    int flag;       /**< 窗口标志 */
} WindowInfo;

/**
 * @brief 初始化窗口
 * @param width 窗口宽度
 * @param height 窗口高度
 * @param title 窗口标题
 * @param flags 窗口配置标志
 */
void init_window(int width, int height, const char * title, int flags);

/**
 * @brief 关闭窗口
 */
void close_window(void);

/**
 * @brief 检查窗口是否就绪
 * @return 窗口已就绪返回true，否则返回false
 */
bool is_window_ready(void);

/**
 * @brief 检查窗口是否应该关闭
 * @return 窗口应关闭返回true，否则返回false
 */
bool window_should_close(void);

/**
 * @brief 设置窗口关闭标志
 */
void window_set_close(void);

/**
 * @brief 获取窗口信息
 * @return WindowInfo结构体
 */
WindowInfo get_window_info(void);

/**
 * @brief 获取按下的按键
 * @return 按键码，无按键时返回0
 */
int get_key_pressed(void);

/**
 * @brief 日志记录函数
 * @param level 日志级别
 * @param text 格式化字符串
 * @param ... 可变参数
 */
void logger(int level, const char *text, ...);

/**
 * @brief 设置日志级别
 * @param level 日志级别
 */
void set_log_level(int level);


#pragma region "plugin"

/**
 * @brief 平台服务接口结构体
 * @details 提供插件所需的各类平台服务函数指针
 */
typedef struct PlatformServices {
    WindowInfo (*get_window_info)(void);   /**< 获取窗口信息 */
    void (*window_set_close)(void);        /**< 设置窗口关闭标志 */

    void* (*malloc)(size_t size);          /**< 内存分配 */
    void* (*calloc)(size_t count, size_t size); /**< 内存分配并清零 */
    void* (*realloc)(void* ptr, size_t size); /**< 重新分配内存 */
    void (*free)(void* ptr);               /**< 释放内存 */

    int (*get_key_pressed)(void);          /**< 获取按下的按键 */

    void (*log)(int level, const char* fmt, ...); /**< 日志记录 */
    void (*set_log_level)(int level);      /**< 设置日志级别 */

    double (*get_time)(void);              /**< 获取当前时间（秒） */
    int (*random_int)(int min, int max);   /**< 生成随机整数 */
    float (*random_float)(float min, float max); /**< 生成随机浮点数 */
} PlatformServices;

/**
 * @brief 获取平台服务接口指针
 * @return PlatformServices结构体指针（单例）
 */
PlatformServices *get_platform_services();

#pragma endregion