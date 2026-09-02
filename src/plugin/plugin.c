#include <dlfcn.h>
#include <sys/cdefs.h>
#include <assert.h>

#include "plugin.h"

#include "platform/platform.h"

#include "version.h"

#define BUF_SIZE 1024 * 1024 // 1MB

typedef struct Plugin {
    void *handle;
    const char *filename;

    bool inited;

    PlatformServices *svc;
    PluginApi *api;
    PluginPri *pri;
} Plugin;

int copy_file(const char *src, const char *dst)
{
    FILE *in = fopen(src, "rb");
    if (!in)
        return -1;

    FILE *out = fopen(dst, "wb");
    if (!out)
        return -1;

    char *buf = malloc(BUF_SIZE);
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

bool load_plugin(const char *filename, Plugin *plugin)
{
    copy_file(filename, COPY_PLUIN_NAME);

    plugin->handle = dlopen(COPY_PLUIN_NAME, RTLD_NOW);
    if (!plugin->handle)
        return false;

    plugin->api = dlsym(plugin->handle, "plugin_api");
    if (!plugin->api)
        return false;

    return true;
}

void unload_plugin(void *handle)
{
    dlclose(handle);
}

Plugin* plugin_init(const char *filename, PlatformServices *svc)
{
    Plugin *plugin = svc->calloc(1, sizeof(Plugin));
    if(plugin == NULL)  goto err0;

    if(!load_plugin(filename, plugin))
    {
        svc->log(7, "load err\n");
        goto err1;
    }

    plugin->filename = filename;
    plugin->svc = svc;
    plugin->api->init(svc);
    plugin->inited = true;

    return plugin;

err1:
    free(plugin);
err0:
    return NULL;
}

bool plugin_is_ready(Plugin *plugin)
{
    return plugin->inited;
}

const char *plugin_get_filename(Plugin *plugin)
{
    return plugin->filename;
}

const char *plugin_get_name(Plugin *plugin)
{
    return plugin->api->name;
}

const Version *plugin_get_version(Plugin *plugin)
{
    return &plugin->api->version;
}

PluginPri * plugin_pre_reload(Plugin *plugin)
{
    return plugin->api->pre_reload();
}

bool plugin_post_reload(Plugin *plugin, PluginPri *pri)
{
    return plugin->api->post_reload(pri);
}

void plugin_render(Plugin *plugin)
{
    plugin->api->render();
}

void plugin_update(Plugin *plugin)
{
    int key = plugin->svc->get_key_pressed();

    if (key != 82) {
        key = plugin->api->key_map(key);
        plugin->api->update(key);
    } else {
        plugin->inited = false;
        plugin->pri = plugin->api->pre_reload();

        unload_plugin(plugin->handle);

        if (!load_plugin(plugin->filename, plugin)) {
            plugin->svc->log(7, "plugin %s reload err!!\n", plugin->filename);
            // TODO 插件加载错误处理
            exit(0);
        }

        if (!plugin->api->post_reload(plugin->pri)) {
            plugin->svc->log(7, "plugin %s reload err!!\n", plugin->filename);
            // TODO 数据加载错误处理
            exit(0);
        }

        plugin->inited = true;
    }
}

void plugin_exit(Plugin *plugin)
{
    assert(plugin->inited);
    plugin->api->exit();
    unload_plugin(plugin->handle);
    plugin->svc->free(plugin);
}