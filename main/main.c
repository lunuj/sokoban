#include "platform/platform.h"
#include "plugin/plugin_api.h"
#include "scene/scene_manager.h"

extern PluginApi PLUGIN_API_NAME;

int main()
{
    platform_init_window("test", 600, 400, 0);

    plugin_api.init(platform_get_api());
    while (!platform_get_window_state())
    {
        plugin_api.render();
        plugin_api.update(platform_get_key_pressed());
    }
    plugin_api.exit();

    platform_close_window();

    return 0;
}
