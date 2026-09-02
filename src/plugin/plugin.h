#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "version.h"

#include "platform/platform.h"

#define COPY_PLUIN_NAME "output/libplug_copy.so"
#define PLUGIN_API_NAME plugin_api

typedef struct Plugin Plugin;
typedef struct PluginPri PluginPri;

typedef struct PluginApi
{
    const char *name;
    const Version version;

    void (*init)(PlatformServices *svc);
    void (*exit)();

    PluginPri *(*pre_reload)(void);
    bool (*post_reload)(PluginPri *pri);

    void (*render)();
    void (*update)(int key);

    int (*key_map)(int key);
} PluginApi;

Plugin * plugin_init(const char * filename, PlatformServices * svc);
bool plugin_is_ready(Plugin * plugin);
const char *plugin_get_filename(Plugin *plugin);
const char *plugin_get_name(Plugin *plugin);
const Version *plugin_get_version(Plugin *plugin);
PluginPri * plugin_pre_reload(Plugin *plugin);
bool plugin_post_reload(Plugin *plugin, PluginPri *in);
void plugin_render(Plugin * plugin);
void plugin_update(Plugin *plugin);
void plugin_exit(Plugin *plugin);