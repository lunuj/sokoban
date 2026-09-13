/**
 * @file scene.h
 * @brief 场景管理模块头文件
 * @details 定义场景类型、操作接口和管理函数
 */

#pragma once
#include <assert.h>

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
    void (*init)(Scene* scene);

    /**
     * @brief 场景重置
     * @param scene 场景指针
     */
    void (*reset)(Scene* scene);

    /**
     * @brief 场景更新
     * @param scene 场景指针
     * @param c 操作码
     */
    void (*update)(Scene* scene, int c);

    /**
     * @brief 场景渲染
     * @param scene 场景指针
     */
    void (*render)(Scene* scene);

    /**
     * @brief 场景退出清理
     * @param scene 场景指针
     */
    void (*exit)(Scene* scene);
} SceneOps;

typedef struct SceneManager SceneManager;

/**
 * @brief 场景结构体
 * @details 包含场景状态、操作函数、叠加层和私有数据
 */
typedef struct Scene
{
    int status;

    SceneManager* mgr;
    const SceneOps* ops;
    void* pri;
} Scene;

typedef enum SceneStatus
{
    SCENE_NONE,
    SCENE_INITED,
    SCENE_RENDER,
    SCENE_UPDATE,
    SCENE_EXIT,
} SceneStatus;

static inline void scene_init(Scene* scene)
{
    assert(scene);
    scene->status = SCENE_INITED;
    scene->ops->init(scene);
}

static inline void scene_reset(Scene* scene)
{
    assert(scene);
    scene->status = SCENE_INITED;
    scene->ops->reset(scene);
}

static inline void scene_exit(Scene* scene)
{
    assert(scene);
    scene->status = SCENE_EXIT;
    scene->ops->exit(scene);
}

static inline void scene_render(Scene* scene)
{
    assert(scene);
    scene->status = SCENE_RENDER;
    scene->ops->render(scene);
}

static inline void scene_update(Scene* scene, int key)
{
    assert(scene);
    scene->status = SCENE_UPDATE;
    scene->ops->update(scene, key);
}

static inline void scene_set_status(Scene* scene, SceneStatus status)
{
    assert(scene);
    scene->status = status;
}

static inline SceneStatus scene_get_status(Scene* scene)
{
    assert(scene);
    return scene->status;
}