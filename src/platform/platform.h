/**
 * @file platform.h
 * @brief 平台服务模块头文件
 * @details 提供窗口管理、输入处理、日志记录和平台服务接口
 */

#pragma once
#include <stdint.h>

#include "platform/platform_def.h"

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
void logger(PlatformLogLevel level, const char *text, ...);

/**
 * @brief 设置日志级别
 * @param level 日志级别
 */
void set_log_level(int level);


#pragma region "plugin"

/**
 * @brief 获取平台服务接口指针
 * @return PlatformServices结构体指针（单例）
 */
PlatformServices *get_platform_services();

#pragma endregion