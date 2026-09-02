
#include "version.h"

#include "platform/platform.h"

#include "plugin/plugin.h"

Plugin *box;

int main()
{
    int key = 0;
    int ret = 0;

    set_log_level(4);
    logger(4, "game start!!\n");

    init_window(600, 400, VERSION_STR, 0);

    box = plugin_init("output/libplug.so", get_platform_services());
    if (box == NULL) goto err;

    printf("filename: %s\n", plugin_get_filename(box));
    printf("plugin name: %s\n", plugin_get_name(box));
    const Version *version = plugin_get_version(box);
    printf("version: %s\n", version_get_string(version));

    while (!window_should_close()) {
        plugin_render(box);
        plugin_update(box);
    }

    plugin_exit(box);

    close_window();
err:
    return 0;
}