
#include "version.h"

#include "platform/platform.h"

#include "plugin/plugin.h"

Plugin* box;

int main()
{
    int key = 0;
    int ret = 0;

    platform_set_log_level(LOG_INFO);
    platform_log(LOG_INFO, "game start!!");

    platform_init_window(version_str, 600, 400, 0);

    box = plugin_init("output/libplug.so", platform_get_api());
    if (box == NULL)
        goto err;

    platform_log(LOG_INFO, "filename: %s", plugin_get_filename(box));
    platform_log(LOG_INFO, "plugin name: %s", plugin_get_name(box));
    const Version* version = plugin_get_version(box);
    platform_log(LOG_INFO, "version: %s", version_get_string(version));

    while (!platform_get_window_state())
    {
        plugin_render(box);
        plugin_update(box);
    }

    plugin_exit(box);

    platform_close_window();
    platform_log(LOG_INFO, "game exit!!");
err:
    return 0;
}