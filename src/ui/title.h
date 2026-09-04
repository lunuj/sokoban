/**
 * @file title.h
 * @brief 标题界面模块头文件
 * @details 提供标题界面的操作函数集获取接口
 */

#pragma once
#include "scene/scene.h"

/**
 * @brief 获取标题界面操作函数集
 * @return SceneOps结构体，包含标题界面的初始化、重置、渲染、更新和退出函数
 */
SceneOps title_get_ops();