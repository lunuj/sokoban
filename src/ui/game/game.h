/**
 * @file game.h
 * @brief 游戏主界面模块头文件
 * @details 提供游戏界面的操作函数集获取接口
 */

#pragma once
#include "scene/scene.h"

/**
 * @brief 获取游戏界面操作函数集
 * @return SceneOps结构体，包含游戏界面的初始化、重置、渲染、更新和退出函数
 */
SceneOps game_get_ops();