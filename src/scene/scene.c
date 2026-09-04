/**
 * @file scene.c
 * @brief 场景管理模块实现
 * @details 管理游戏场景的初始化、切换、渲染、更新和生命周期
 */

#include <stddef.h>

#include "raylib/raylib.h"
#include "scene.h"

#include "ui/game/game.h"
#include "ui/game/end.h"
#include "ui/game/menu.h"
#include "ui/title.h"

#include "plugin/plugin.h"

/**
 * @brief 插件私有数据（场景管理器）
 * @details 保存所有场景实例和当前场景索引
 */
struct PluginPri {
    uint32_t magic;                     /**< 魔数，用于验证数据完整性 */
    Version verion;                     /**< 插件版本信息 */
    PlatformServices *svc;              /**< 平台服务接口指针 */

    int current;                        /**< 当前场景索引 */
    Scene scene_list[SCENE_MAX];        /**< 场景数组 */
} *pri;

/**
 * @brief 获取当前场景指针
 * @return 当前场景指针
 */
Scene *get_current()
{
    return pri->scene_list + pri->current;
}

/**
 * @brief 根据索引获取场景指针
 * @param index 场景索引
 * @return 场景指针
 */
static Scene *get_scene(int index)
{
    return pri->scene_list + index;
}

/**
 * @brief 设置当前场景
 * @param index 场景索引
 */
void scene_set_current(int index)
{
    pri->current = index;
}

/**
 * @brief 场景系统初始化
 * @param service 平台服务接口指针
 * @note 初始化所有场景并设置默认当前场景为标题界面
 */
// TODO 子场景下函数调用，资源申请问题
void scene_init(PlatformServices *service)
{
    pri = service->calloc(1, sizeof(PluginPri));
    pri->magic = 0xdeadbeef;
    pri->verion = version_get_current();
    pri->svc = service;

    // TODO 场景创建问题，不希望scene管理子场景，而是只提供一个通用框架
    SceneOps ops = title_get_ops();
    ops.init(pri->scene_list + TITLE);
    pri->current = TITLE;

    ops = game_get_ops();
    ops.init(pri->scene_list + GAME);
    ops = menu_get_ops();
    ops.init(pri->scene_list + GAME_MENU);
    ops = end_get_ops();
    ops.init(pri->scene_list + GAME_END);
    pri->current = TITLE;
}

/**
 * @brief 重置指定场景
 * @param index 场景索引
 */
void scene_reset(int index)
{
    Scene *scene = get_scene(index);

    scene->ops.reset(scene);
    scene->overlay = NULL;
    scene->end = false;
}

/**
 * @brief 渲染当前场景
 * @details 先渲染当前场景，再渲染叠加层（如果有）
 */
void scene_render()
{
    Scene *scene = get_current();

    BeginDrawing();
    // TODO overlay可能也有overlay
    scene->ops.render(scene);
    if (scene->overlay != NULL) scene->overlay->ops.render(scene->overlay);
    EndDrawing();
}

/**
 * @brief 按键映射函数
 * @param c 原始按键值
 * @return 映射后的操作码
 * @details 将键盘按键映射为统一的选项操作码
 */
int scene_key_map(int c)
{
    int ret = OPTION_NONE;
    switch (c) {
    case KEY_W:
    case KEY_UP:
        ret = OPTION_MOV_UP;
        break;
    case KEY_S:
    case KEY_DOWN:
        ret = OPTION_MOV_DOWN;
        break;
    case KEY_A:
    case KEY_LEFT:
        ret = OPTION_MOV_LEFT;
        break;
    case KEY_D:
    case KEY_RIGHT:
        ret = OPTION_MOV_RIGHT;
        break;
    case KEY_ESCAPE:
        ret = OPTION_BACK;
        break;
    case KEY_ENTER:
        ret = OPTION_ENTER;
        break;
    case KEY_E:
        ret = OPTION_UNDO;
        break;
    case KEY_R:
        ret = OPTION_RELOAD;
    default:
        break;
    }
    return ret;
}

/**
 * @brief 更新场景状态
 * @param key 按键操作码
 * @details 处理叠加层逻辑，更新当前场景或触发窗口关闭
 */
void scene_update(int key)
{
    Scene *scene = get_current();

    if (scene->overlay != NULL) {
        if (scene->overlay->end) {
            scene->overlay = NULL;
        } else {
            scene = scene->overlay;
        }
    }

    if (scene->end)
        pri->svc->window_set_close();
    else
        scene->ops.update(scene, key);
}

/**
 * @brief 设置场景结束状态
 * @param index 场景索引
 * @param status 结束状态（true=结束）
 */
void scene_set_status(int index, int status)
{
    Scene *scene = get_scene(index);

    scene->end = (bool)status;
}

/**
 * @brief 获取场景结束状态
 * @param index 场景索引
 * @return 结束状态（true=结束）
 */
bool scene_get_status(int index)
{
    Scene *scene = get_scene(index);

    return scene->end;
}

/**
 * @brief 退出场景系统
 * @details 清理所有场景并释放资源
 */
void scene_exit()
{
    for (int i = 0; i < SCENE_MAX; i++) {
        pri->scene_list[i].ops.exit(pri->scene_list + i);
    }
    pri->svc->free(pri);
}

/**
 * @brief 设置叠加层
 * @param index 基础场景索引
 * @param overlay 叠加场景索引
 */
void scene_set_overlay(int index, int overlay)
{
    Scene *scene = get_current();

    scene = get_scene(index);
    scene->overlay = get_scene(overlay);
}

/**
 * @brief 清除叠加层
 * @param index 场景索引
 */
void scene_clr_overlay(int index)
{
    Scene *scene = get_scene(index);

    scene->overlay = NULL;
}

/**
 * @brief 插件重载前回调
 * @return 场景管理器数据指针
 */
PluginPri *scene_pre_reload(void)
{
    return pri;
}

/**
 * @brief 插件重载后回调
 * @param plugin 重载前保存的数据指针
 * @return 版本兼容返回true，否则返回false
 */
bool scene_post_reload(PluginPri *plugin)
{
    pri = plugin;
    Version v = version_get_current();
    pri->svc->log(7, "current version: %s\n", version_get_string(&v));
    pri->svc->log(7, "plugin version: %s\n", version_get_string(&pri->verion));
    return version_is_compatible(&v, &pri->verion);
}

/**
 * @brief 插件API导出符号
 * @details 供plugin模块加载使用
 */
PluginApi PLUGIN_API_NAME = {
    .name = "scene",
    .version = VERSION,

    .init = scene_init,
    .exit = scene_exit,

    .pre_reload = scene_pre_reload,
    .post_reload = scene_post_reload,

    .render = scene_render,
    .update = scene_update,

    .key_map = scene_key_map,
};