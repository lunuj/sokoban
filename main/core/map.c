#include "sokoban/core/map.h"
#include "sokoban/tool/xsb.h"

int main()
{
    Map map;

    load_map_from_xsb("res/map/map01.txt", &map);
    save_map("map.bin", &map);

    load_map("1.bin", &map);
    save_map_to_xsb("1.txt", &map);

    return 0;
}