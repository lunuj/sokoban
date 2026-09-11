/* 本文件由 gen_from_impl_h.py 自动生成, 请勿手动修改 */

#include "platform_plugin.h"
#include "platform_api.h"

static PlatformApi *platform;

bool plugin_init_window(const char* title, int width, int height, int flags)
{
    return platform->init_window(title, width, height, flags);
}

void plugin_close_window(void)
{
    return platform->close_window();
}

void plugin_set_window_close(void)
{
    return platform->set_window_close();
}

bool plugin_get_window_state(void)
{
    return platform->get_window_state();
}

int plugin_get_window_width(void)
{
    return platform->get_window_width();
}

int plugin_get_window_height(void)
{
    return platform->get_window_height();
}

int plugin_get_key_pressed(void)
{
    return platform->get_key_pressed();
}

void plugin_begin_drawing(void)
{
    return platform->begin_drawing();
}

void plugin_end_drawing(void)
{
    return platform->end_drawing();
}

void plugin_clear_background(Color color)
{
    return platform->clear_background(color);
}

Texture plugin_load_texture(const char* filename)
{
    return platform->load_texture(filename);
}

void plugin_unload_texture(Texture texture)
{
    return platform->unload_texture(texture);
}

void plugin_draw_texture(Texture texture, int posX, int posY, Color tint)
{
    return platform->draw_texture(texture, posX, posY, tint);
}

void plugin_draw_texture_pro(Texture texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    return platform->draw_texture_pro(texture, source, dest, origin, rotation, tint);
}

void plugin_draw_rectangle(int posX, int posY, int width, int height, Color color)
{
    return platform->draw_rectangle(posX, posY, width, height, color);
}

void plugin_draw_text(const char* text, int posX, int posY, int font_size, Color color)
{
    return platform->draw_text(text, posX, posY, font_size, color);
}

void plugin_log(int level, const char* fmt, ...)
{
    return platform->log(level, fmt);
}

void plugin_set_log_level(int level)
{
    return platform->set_log_level(level);
}

void* plugin_calloc(size_t count, size_t size)
{
    return platform->calloc(count, size);
}

void plugin_free(void* ptr)
{
    return platform->free(ptr);
}

void plugin_set_api(PlatformApi *api)
{
    platform = api;
}
