/**
 * @file core.h
 * @brief 游戏核心模块头文件
 * @details 提供地图数据的加载和保存函数接口
 */

#pragma once
#include "core.h"

/**
 * @brief 从文本文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int load_map_from_txt(const char *filename, Map *map);

/**
 * @brief 从二进制文件加载地图
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int load_map_from_bin(const char *filename, Map *map);

/**
 * @brief 将地图保存为文本文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int save_map_to_txt(const char *filename, Map *map);

/**
 * @brief 将地图保存为二进制文件
 * @param filename 文件名
 * @param map 地图结构体指针
 * @return 成功返回0，失败返回-1
 */
int save_map_to_bin(const char *filename, Map *map);