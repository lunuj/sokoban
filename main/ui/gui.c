#include "platform/platform.h"
#include "ui/scene.h"

int main()
{
    init_window(600, 400, "test", 0);

    scene_init(get_platform_services());
    while (!window_should_close()) {
        scene_render();
        scene_update(scene_key_map(get_key_pressed()));
    }
    scene_exit();

    close_window();

    return 0;
}
