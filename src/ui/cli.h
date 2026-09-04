/**
 * @file cli.h
 * @brief 命令行界面模块头文件
 * @details 提供地图字符转换、显示和命令行输入函数接口
 */

#pragma once

/**
 * @brief 将地图类型转换为显示字符
 * @param maptype 地图类型（位掩码）
 * @return 对应的显示字符
 */
int map_to_cli(int maptype);

/**
 * @brief 将显示字符转换为地图类型
 * @param ch 显示字符
 * @return 对应的地图类型（位掩码）
 */
int cli_to_map(int ch);

/**
 * @brief 在命令行显示地图
 * @param map 地图结构体指针
 */
void cli_show(Map *map);

/**
 * @brief 从命令行获取用户输入
 * @return 对应的命令操作码
 */
int cli_get();