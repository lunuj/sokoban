#include <stdio.h>
#include "core.h"
#include "tool/map.h"

int main()
{
    Map map;
    load_map_from_txt("res/map/map.txt", &map);
    save_map_to_bin("1.bin", &map);
    load_map_from_bin("1.bin", &map);
    save_map_to_txt("1.txt", &map);
    return 0;
}