/**
 * @file menu.h
 * @brief 游戏菜单模块头文件
 * @details 提供游戏菜单界面的操作函数集获取接口
 */

#pragma once
#include "scene/scene.h"

/**
 * @brief 获取菜单界面操作函数集
 * @return SceneOps结构体，包含菜单界面的初始化、重置、渲染、更新和退出函数
 */
SceneOps menu_get_ops();