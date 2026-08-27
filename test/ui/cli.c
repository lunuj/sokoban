#include "std.h"

#include "core.h"
#include "tool/map.h"
#include "ui/cli.h"

extern Map map;

int main()
{
    load_map_from_txt("res/map/map01.txt", &map);
    map.is_recording = true;
    while (1)
    {
        cli_show(&map);
        if (is_win(&map))
        {
            printf("player win!\n");
            break;
        }
        if (update(&map, cli_get()) < 0)
            break;
    }

    return 0;
}