/**
 * @file plugin.h
 * @brief 插件管理模块头文件
 * @details 定义插件API结构体和插件管理函数接口
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "version.h"

#include "plugin_def.h"
#include "platform/platform_def.h"

/** @brief 复制后的插件动态库路径 */
#define COPY_PLUIN_NAME "output/libplug_copy.so"

/**
 * @brief 初始化插件
 * @param filename 插件文件名
 * @param svc 平台服务接口指针
 * @return 成功返回Plugin结构体指针，失败返回NULL
 */
Plugin * plugin_init(const char * filename, PlatformServices * svc);

/**
 * @brief 检查插件是否就绪
 * @param plugin 插件结构体指针
 * @return 已初始化返回true，否则返回false
 */
bool plugin_is_ready(Plugin * plugin);

/**
 * @brief 获取插件文件名
 * @param plugin 插件结构体指针
 * @return 插件文件名
 */
const char *plugin_get_filename(Plugin *plugin);

/**
 * @brief 获取插件名称
 * @param plugin 插件结构体指针
 * @return 插件名称
 */
const char *plugin_get_name(Plugin *plugin);

/**
 * @brief 获取插件版本信息
 * @param plugin 插件结构体指针
 * @return 版本结构体指针
 */
const Version *plugin_get_version(Plugin *plugin);

/**
 * @brief 执行插件重载前操作
 * @param plugin 插件结构体指针
 * @return 重载前保存的数据指针
 */
PluginPri * plugin_pre_reload(Plugin *plugin);

/**
 * @brief 执行插件重载后操作
 * @param plugin 插件结构体指针
 * @param in 重载前保存的数据指针
 * @return 成功返回true，失败返回false
 */
bool plugin_post_reload(Plugin *plugin, PluginPri *in);

/**
 * @brief 渲染插件
 * @param plugin 插件结构体指针
 */
void plugin_render(Plugin * plugin);

/**
 * @brief 更新插件状态
 * @param plugin 插件结构体指针
 */
void plugin_update(Plugin *plugin);

/**
 * @brief 退出并清理插件
 * @param plugin 插件结构体指针
 */
void plugin_exit(Plugin *plugin);