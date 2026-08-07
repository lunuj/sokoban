#pragma once
#include "core.h"

int load_map_from_txt(const char *filename, Map *map);
int load_map_from_bin(const char *filename, Map *map);
int save_map_to_txt(const char *filename, Map *map);
int save_map_to_bin(const char *filename, Map *map);