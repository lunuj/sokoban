#pragma once

typedef struct Map Map;

int map_to_xsb(int maptype);
int xsb_to_map(int ch);
int save_map_to_xsb(const char* filename, Map* map);
int load_map_from_xsb(const char* filename, Map* map);
