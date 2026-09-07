#pragma once

/**
 * @brief 插件结构体类型（不透明指针）
 * @details 内部实现在plugin_api.h中
 */
typedef struct Plugin Plugin;

/**
 * @brief 插件重载前数据类型（不透明指针）
 * @details 由具体插件定义其内容
 */
typedef struct PluginPri PluginPri;

/**
 * @brief 平台提供的服务类型（不透明指针）
 * @details 由platform定义其内容
 */
typedef struct PlatformServices PlatformServices;