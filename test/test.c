#include <stdbool.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>

#include "plug.h"
#include "raylib/raylib.h"

#define MAP_WIDTH 40
#define MAP_HEIGHT 22.5
#define MAP_BLOCK_SIZE 10

void *lib = NULL;
void (*lib_init)() = NULL;
void *(*lib_pre_reload)() = NULL;
void (*lib_post_reload)(void *state) = NULL;
void (*lib_update)(Env *env) = NULL;
void (*lib_exit)() = NULL;

static void *dlopen(const char *path, int flag)
{
    (void)flag;
    return LoadLibraryA(path);
}

static void *dlsym(void *lib, const char *name)
{
    return (void *)GetProcAddress(lib, name);
}

static void dlclose(void *lib)
{
    FreeLibrary(lib);
}

char *path = "output/libplug.so";

#define BUF_SIZE 1024 * 1024 // 1MB

int copy_file(const char *src, const char *dst)
{
    FILE *in = fopen(src, "rb");
    if (!in)
        return -1;

    FILE *out = fopen(dst, "wb");
    if (!out)
        return -1;

    char *buf = malloc(BUF_SIZE);
    if (!buf)
        return -1;

    size_t n;

    while ((n = fread(buf, 1, BUF_SIZE, in)) > 0)
    {
        fwrite(buf, 1, n, out);
    }

    fclose(in);
    fclose(out);
    free(buf);

    return 0;
}

bool reload_libplug()
{
    if (lib != NULL)
    {
        dlclose(lib);
    }

    copy_file("output/libplug.so", "output/libplug_copy.so");

    lib = dlopen("output/libplug_copy.so", 0);
    if (!lib)
        return false;
    lib_init = dlsym(lib, "plug_init");
    if (!lib_init)
        return false;
    lib_pre_reload = dlsym(lib, "plug_pre_reload");
    if (!lib_pre_reload)
        return false;
    lib_post_reload = dlsym(lib, "plug_post_reload");
    if (!lib_post_reload)
        return false;
    lib_update = dlsym(lib, "plug_update");
    if (!lib_update)
        return false;
    lib_exit = dlsym(lib, "plug_exit");
    if (!lib_exit)
        return false;

    return true;
}

int main()
{
    bool pause = false;
    Env env = {0};

    if (!reload_libplug())
        return 1;

    lib_init();

    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TRANSPARENT);

    InitWindow(MAP_WIDTH * MAP_BLOCK_SIZE, MAP_HEIGHT * MAP_BLOCK_SIZE, "Hello Raylib");
    InitAudioDevice();

    Music m = LoadMusicStream("res/target2.ogg");
    m.looping = false;
    PlayMusicStream(m);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R))
        {
            void *state = lib_pre_reload();
            if (!reload_libplug())
                return 2;
            lib_post_reload(state);
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            if (env.record)
                env.audio_index = 0;
            PlayMusicStream(m);
            pause = !pause;
        }

        if (IsKeyPressed(KEY_E))
        {
            env.record = !env.record;
            if (env.record)
            {
                env.f_video = popen(
                    "ffmpeg "
                    "-loglevel verbose "
                    "-y "
                    "-f rawvideo "
                    "-pix_fmt rgba "
                    "-s 400x225 "
                    "-r 60 "
                    "-i - "
                    "-c:v libx264 "
                    "-vb 2500k "
                    "-c:a aac "
                    "-ab 200k "
                    "-preset fast "
                    "-crf 23 "
                    "-f mp4 "
                    "output.mp4",
                    "wb"
                );
                env.f_audio = popen(
                    "ffmpeg "
                    "-loglevel verbose "
                    "-y "
                    "-f s16le "
                    "-ar 44100 "
                    "-ac 2 "
                    "-i - "
                    "output.wav",
                    "wb"
                );
            }
            else
            {
                pclose(env.f_video);
                pclose(env.f_audio);
                system("ffmpeg -i output.mp4 -i output.wav -c:v copy -c:a copy out.mp4");
            }
        }

        env.dt = pause ? 0 : GetFrameTime();
        env.w = GetScreenWidth();
        env.h = GetScreenHeight();

        lib_update(&env);
        UpdateMusicStream(m);
    }
    StopMusicStream(m);
    CloseAudioDevice();
    CloseWindow();
    lib_exit();

    return 0;
}