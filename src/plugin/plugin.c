/**
 * @file plugin.c
 * @brief 插件管理模块实现
 * @details 提供插件的加载、卸载、重载、更新和渲染等功能
 */

#include <assert.h>
#include <dlfcn.h>

#include "plugin.h"
#include "plugin_api.h"

#include "platform/platform.h"

#include "version.h"

/** @brief 文件复制缓冲区大小：1MB */
#define BUF_SIZE 1024 * 1024 // 1MB

/**
 * @brief 插件结构体
 * @details 封装了插件句柄、状态、服务接口和API
 */
typedef struct Plugin
{
    void* handle;         /**< 动态库句柄 */
    const char* filename; /**< 插件文件名 */

    bool inited; /**< 插件是否已初始化 */

    PluginApi* api; /**< 插件API接口指针 */
    void* pri;      /**< 插件重载前数据指针 */
} Plugin;

/**
 * @brief 复制文件
 * @param src 源文件路径
 * @param dst 目标文件路径
 * @return 成功返回0，失败返回-1
 */
int copy_file(const char* src, const char* dst)
{
    FILE* in = fopen(src, "rb");
    if (!in)
        return -1;

    FILE* out = fopen(dst, "wb");
    if (!out)
        return -1;

    char* buf = malloc(BUF_SIZE);
    if (!buf)
        return -1;

    size_t n;

    while ((n = fread(buf, 1, BUF_SIZE, in)) > 0)
    {
        fwrite(buf, 1, n, out);
    }

    fclose(in);
    fclose(out);
    free(buf);

    return 0;
}

/**
 * @brief 加载插件动态库
 * @param filename 插件文件名
 * @param plugin 插件结构体指针
 * @return 加载成功返回true，失败返回false
 */
bool load_plugin(const char* filename, Plugin* plugin)
{
    copy_file(filename, COPY_PLUIN_NAME);

    plugin->handle = dlopen(COPY_PLUIN_NAME, RTLD_NOW);
    if (!plugin->handle)
    {
        dlclose(plugin->handle);
        return false;
    }

    plugin->api = dlsym(plugin->handle, "plugin_api");
    if (!plugin->api)
        return false;

    return true;
}

/**
 * @brief 卸载插件动态库
 * @param handle 动态库句柄
 */
void unload_plugin(void* handle)
{
    dlclose(handle);
}

/**
 * @brief 初始化插件
 * @param filename 插件文件名
 * @param api 平台服务接口指针
 * @return 成功返回Plugin结构体指针，失败返回NULL
 */
Plugin* plugin_init(const char* filename, PlatformApi* api)
{
    Plugin* plugin = platform_calloc(1, sizeof(Plugin));
    if (plugin == NULL)
        goto err0;

    if (!load_plugin(filename, plugin))
    {
        platform_log(LOG_FATAL, "load err\n");
        goto err1;
    }

    plugin->filename = filename;
    plugin->api->init(api);
    plugin->inited = true;

    return plugin;

err1:
    platform_free(plugin);
err0:
    return NULL;
}

/**
 * @brief 检查插件是否就绪
 * @param plugin 插件结构体指针
 * @return 已初始化返回true，否则返回false
 */
bool plugin_is_ready(Plugin* plugin)
{
    return plugin->inited;
}

/**
 * @brief 获取插件文件名
 * @param plugin 插件结构体指针
 * @return 插件文件名
 */
const char* plugin_get_filename(Plugin* plugin)
{
    return plugin->filename;
}

/**
 * @brief 获取插件名称
 * @param plugin 插件结构体指针
 * @return 插件名称
 */
const char* plugin_get_name(Plugin* plugin)
{
    return plugin->api->name;
}

/**
 * @brief 获取插件版本信息
 * @param plugin 插件结构体指针
 * @return 版本结构体指针
 */
const Version* plugin_get_version(Plugin* plugin)
{
    return plugin->api->version;
}

/**
 * @brief 执行插件重载前操作
 * @param plugin 插件结构体指针
 * @return 重载前保存的数据指针
 */
void* plugin_pre_reload(Plugin* plugin)
{
    return plugin->api->pre_reload();
}

/**
 * @brief 执行插件重载后操作
 * @param plugin 插件结构体指针
 * @param pri 重载前保存的数据指针
 * @return 成功返回true，失败返回false
 */
bool plugin_post_reload(Plugin* plugin, void* pri)
{
    return plugin->api->post_reload(pri);
}

/**
 * @brief 渲染插件
 * @param plugin 插件结构体指针
 */
void plugin_render(Plugin* plugin)
{
    plugin->api->render();
}

/**
 * @brief 更新插件状态
 * @details 处理键盘输入，当按下'R'键(ASCII 82)时触发插件热重载
 * @param plugin 插件结构体指针
 */
void plugin_update(Plugin* plugin)
{
    int key = platform_get_key_pressed();

    if (key != 82)
    {
        plugin->api->update(key);
    }
    else
    {
        plugin->inited = false;
        plugin->pri = plugin->api->pre_reload();

        unload_plugin(plugin->handle);

        if (!load_plugin(plugin->filename, plugin))
        {
            platform_log(LOG_FATAL, "plugin %s reload err!!\n", plugin->filename);
            // TODO 插件加载错误处理
            exit(0);
        }

        if (!plugin->api->post_reload(plugin->pri))
        {
            platform_log(LOG_FATAL, "plugin %s reload err!!\n", plugin->filename);
            // TODO 数据加载错误处理
            exit(0);
        }

        plugin->inited = true;
    }
}

/**
 * @brief 退出并清理插件
 * @param plugin 插件结构体指针
 * @note 插件必须处于已初始化状态
 */
void plugin_exit(Plugin* plugin)
{
    assert(plugin->inited);
    plugin->api->exit();
    unload_plugin(plugin->handle);
    platform_free(plugin);
}