/**
 * @file scene.h
 * @brief 场景管理模块头文件
 * @details 定义场景类型、操作接口和管理函数
 */

#pragma once

#include "plugin/plugin.h"

/**
 * @brief 操作选项枚举
 * @details 表示用户输入的各种操作
 */
enum Option
{
    OPTION_NONE,        /**< 无操作 */
    OPTION_WIN,         /**< 胜利 */
    OPTION_LOSE,        /**< 失败 */
    OPTION_BACK,        /**< 返回 */
    OPTION_ENTER,       /**< 确认/进入 */
    OPTION_MOV_UP,      /**< 上移 */
    OPTION_MOV_DOWN,    /**< 下移 */
    OPTION_MOV_LEFT,    /**< 左移 */
    OPTION_MOV_RIGHT,   /**< 右移 */
    OPTION_UNDO,        /**< 撤销 */
    OPTION_RELOAD,      /**< 重载 */
};

/**
 * @brief 场景类型枚举
 * @details 定义系统中所有可用的场景类型
 */
enum SceneType
{
    TITLE,      /**< 标题界面 */
    STAGE,      /**< 关卡选择 */
    GAME,       /**< 游戏主界面 */
    GAME_MENU,  /**< 游戏菜单 */
    GAME_END,   /**< 游戏结束界面 */
    SCENE_MAX,  /**< 场景总数 */
};

/**
 * @brief 场景结构体（前向声明）
 */
typedef struct Scene Scene;

/**
 * @brief 场景操作函数集
 * @details 每个场景需要实现这些生命周期函数
 */
typedef struct SceneOps
{
    /**
     * @brief 场景初始化
     * @param scene 场景指针
     */
    void (*init)(Scene *scene);

    /**
     * @brief 场景重置
     * @param scene 场景指针
     */
    void (*reset)(Scene *scene);

    /**
     * @brief 场景更新
     * @param scene 场景指针
     * @param c 操作码
     */
    void (*update)(Scene *scene, int c);

    /**
     * @brief 场景渲染
     * @param scene 场景指针
     */
    void (*render)(Scene *scene);

    /**
     * @brief 场景退出清理
     * @param scene 场景指针
     */
    void (*exit)(Scene *scene);
} SceneOps;

/**
 * @brief 场景结构体
 * @details 包含场景状态、操作函数、叠加层和私有数据
 */
typedef struct Scene
{
    bool end;               /**< 场景是否结束 */
    SceneOps ops;           /**< 场景操作函数集 */
    Scene *overlay;         /**< 叠加层场景指针 */
    void *pri;              /**< 场景私有数据 */
} Scene;


/**
 * @brief 初始化场景系统
 * @param service 平台服务接口指针
 */
void scene_init(PlatformServices *service);

/**
 * @brief 退出场景系统
 * @details 清理所有场景资源
 */
void scene_exit();

/**
 * @brief 渲染当前场景
 */
void scene_render();

/**
 * @brief 更新场景状态
 * @param key 操作码
 */
void scene_update(int key);

/**
 * @brief 重置指定场景
 * @param index 场景索引
 */
void scene_reset(int index);

/**
 * @brief 设置场景结束状态
 * @param index 场景索引
 * @param status 结束状态（true=结束）
 */
void scene_set_status(int index, int status);

/**
 * @brief 获取场景结束状态
 * @param index 场景索引
 * @return 结束状态（true=结束）
 */
bool scene_get_status(int index);

/**
 * @brief 切换当前场景
 * @param index 场景索引
 */
void scene_set_current(int index);

/**
 * @brief 设置叠加层
 * @param index 基础场景索引
 * @param overlay 叠加场景索引
 */
void scene_set_overlay(int index, int overlay);

/**
 * @brief 清除叠加层
 * @param index 场景索引
 */
void scene_clr_overlay(int index);

/**
 * @brief 按键映射函数
 * @param c 原始按键值
 * @return 映射后的操作码
 */
int scene_key_map(int c);