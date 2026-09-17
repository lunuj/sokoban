/* 本文件由 gen_from_impl_h.py 自动生成, 请勿手动修改 */

#include "platform.h"
#include "platform_api.h"
#include "platform_raylib.h"

/* ---- platform_xxx 实现 ---- */
bool platform_init_window(const char* title, int width, int height, int flags)
{
    return impl_init_window(title, width, height, flags);
}

void platform_close_window(void)
{
    impl_close_window();
}

void platform_set_window_close(void)
{
    impl_set_window_close();
}

bool platform_get_window_state(void)
{
    return impl_get_window_state();
}

int platform_get_window_width(void)
{
    return impl_get_window_width();
}

int platform_get_window_height(void)
{
    return impl_get_window_height();
}

int platform_get_key_pressed(void)
{
    return impl_get_key_pressed();
}

void platform_begin_drawing(void)
{
    impl_begin_drawing();
}

void platform_end_drawing(void)
{
    impl_end_drawing();
}

void platform_begin_mode_2d(Camera2D camera)
{
    impl_begin_mode_2d(camera);
}

void platform_end_mode_2d(void)
{
    impl_end_mode_2d();
}

void platform_clear_background(Color color)
{
    impl_clear_background(color);
}

Texture platform_load_texture(const char* filename)
{
    return impl_load_texture(filename);
}

void platform_unload_texture(Texture texture)
{
    impl_unload_texture(texture);
}

void platform_draw_texture(Texture texture, int posX, int posY, Color tint)
{
    impl_draw_texture(texture, posX, posY, tint);
}

void platform_draw_texture_pro(Texture texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
    impl_draw_texture_pro(texture, source, dest, origin, rotation, tint);
}

void platform_draw_rectangle(int posX, int posY, int width, int height, Color color)
{
    impl_draw_rectangle(posX, posY, width, height, color);
}

void platform_draw_text(const char* text, int posX, int posY, int font_size, Color color)
{
    impl_draw_text(text, posX, posY, font_size, color);
}

void platform_log(int level, const char* fmt, ...)
{
    impl_log(level, fmt);
}

void platform_set_log_level(int level)
{
    impl_set_log_level(level);
}

void* platform_calloc(size_t count, size_t size)
{
    return impl_calloc(count, size);
}

void platform_free(void* ptr)
{
    impl_free(ptr);
}

/* ---- PlatformApi 实例 ---- */
static PlatformApi platform = {
    .init_window = platform_init_window,
    .close_window = platform_close_window,
    .set_window_close = platform_set_window_close,
    .get_window_state = platform_get_window_state,
    .get_window_width = platform_get_window_width,
    .get_window_height = platform_get_window_height,
    .get_key_pressed = platform_get_key_pressed,
    .begin_drawing = platform_begin_drawing,
    .end_drawing = platform_end_drawing,
    .begin_mode_2d = platform_begin_mode_2d,
    .end_mode_2d = platform_end_mode_2d,
    .clear_background = platform_clear_background,
    .load_texture = platform_load_texture,
    .unload_texture = platform_unload_texture,
    .draw_texture = platform_draw_texture,
    .draw_texture_pro = platform_draw_texture_pro,
    .draw_rectangle = platform_draw_rectangle,
    .draw_text = platform_draw_text,
    .log = platform_log,
    .set_log_level = platform_set_log_level,
    .calloc = platform_calloc,
    .free = platform_free,
};

PlatformApi* platform_get_api(void)
{
    return &platform;
}
