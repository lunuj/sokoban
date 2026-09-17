#pragma once

#include "plugin_def.h"
#include "version.h"

/** @brief 插件API符号名称 */
#define PLUGIN_API_NAME plugin_api

typedef struct PlatformApi PlatformApi;

/**
 * @brief 插件API结构体
 * @details 插件必须实现此结构体并通过 PLUGIN_API_NAME 符号导出
 */
typedef struct PluginApi
{
    const char* name;      /**< 插件名称 */
    const Version *version; /**< 插件版本信息 */

    /**
     * @brief 插件初始化函数
     * @param api 平台服务接口指针
     */
    void (*init)(PlatformApi* api);

    /**
     * @brief 插件退出清理函数
     */
    void (*exit)();

    /**
     * @brief 插件重载前回调
     * @return 需要保存的插件状态数据指针
     */
    void* (*pre_reload)(void);

    /**
     * @brief 插件重载后回调
     * @param pri 重载前保存的数据指针
     * @return 成功返回true，失败返回false
     */
    bool (*post_reload)(void* pri);

    /**
     * @brief 插件渲染函数
     */
    void (*render)();

    /**
     * @brief 插件更新函数
     * @param key 键盘按键值
     */
    void (*update)(int key);
} PluginApi;