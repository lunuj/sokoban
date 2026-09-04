/**
 * @file end.h
 * @brief 游戏结束界面模块头文件
 * @details 提供游戏结束界面的操作函数集获取接口
 */

#pragma once
#include "scene/scene.h"

/**
 * @brief 获取结束界面操作函数集
 * @return SceneOps结构体，包含结束界面的初始化、重置、渲染、更新和退出函数
 */
SceneOps end_get_ops();