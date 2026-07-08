#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <assert.h> 
#include <math.h> 

#include "raylib/raylib.h"
#include "plug.h"

typedef struct {
    size_t from;
    size_t to;
    float duration;
} Keyframe;

typedef struct {
    size_t i;
    bool loop;
    float duration;
    size_t count;
    Keyframe *kf;
} Animation;

typedef struct {
    size_t size;

    Animation anim[2];
    Font font;
} Plug;

static Plug *p;

void load_resources()
{
    // p->font = LoadFont("res/consolar.ttf");
}

void unload_resources()
{
    // UnloadFont(p->font);
}

size_t keyframe_count = 2;
Keyframe keyframes[8] = {{1, 0, 1}, {0, 0, 1},{1, 1, 1}, {0, 1, 1},{1, 1, 1}, {0, 1, 0.5}, };

Wave w;

void plug_init()
{
    p = malloc(sizeof(*p));
    assert(p != NULL);

    memset(p, 0, sizeof(*p));
    p->size = sizeof(*p);
    p->anim[0].loop = true;
    p->anim[0].kf = keyframes;
    p->anim[0].count = 2;
    p->anim[1].loop = true;
    p->anim[1].kf = keyframes + 2;
    p->anim[1].count = 2;

    w = LoadWave("res/ding.wav");
    load_resources();
}

void *plug_pre_reload()
{
    unload_resources();
    return p;
}

void plug_post_reload(void *state)
{
    p = state;

    if(p->size < sizeof(*p))
    {
        p = realloc(p, sizeof(*p));
        p->size = sizeof(*p);
    }

    load_resources();
}

#define Lerp(from, to, w) (from * (1 - w) + to * w)

float animation_value(Animation *a)
{
    Keyframe *kf = a->kf;
    if(a->i >= a->count) return kf[a->count - 1].to;

    kf += a->i;
    return Lerp(kf->from, kf->to, a->duration/kf->duration);
}

float update_animation(Animation *a, float dt)
{
    if(a->i >= a->count)
    {
        if(a->loop)
        {
            a->i = 0;
            a->duration = 0;
        }
        return animation_value(a);
    }

    Keyframe *kf = a->kf;
    kf += a->i;

    a->duration += dt;

    while(a->duration >= kf->duration)
    {
        a->duration -= kf->duration;
        a->i++;
        if(a->i < a->count)
        {
            kf = a->kf + a->i;
        }
        else
        {
            break;
        }
    }

    return animation_value(a);
}


void show(float dt, float w, float h)
{
    int rw = 80;
    int rh = 45;

    int pad = rw*0.15;
    int font_size = 40;

    ClearBackground(BLANK);

    float t1 = update_animation(p->anim, dt);
    float t2 = update_animation(p->anim + 1, dt);

    for(int i = -3; i < 4; i++)
    {
        int pos = (rw + pad)*t1;
        DrawRectangle(i * (pad + rw) - pos + (w - rw)/2, (h - rh)/2, rw, rh, RAYWHITE);

        Color c = BLACK;
        float _t = t2;

        if(i < 0)
        {
            _t = 0;
        }
        else if(i == 0)
        {
            if(p->anim[1].i != 0)
                _t = t2;
            else
                _t = 0;
        }
        else
        {
            _t = 1;
        }

        DrawTextEx(
            p->font,
            "0", 
            (Vector2){i * (pad + rw) - pos + (w - rw)/2 + rw/2 - font_size/2,
            (h - rh)/2 + rh/2-font_size/2}, font_size*(1-_t), 0, c);
        DrawTextEx(
            p->font,
            "1", 
            (Vector2){i * (pad + rw) - pos + (w - rw)/2 + rw/2 - font_size/2,
            (h - rh)/2 + rh/2-font_size/2}, font_size*(_t), 0, c);
    }

    Rectangle rec = {
        .x = (w - rw - 20)/2,
        .y = (h - rh - 20)/2,
        .width = rw + 20,
        .height = rh + 20,
    };
    DrawRectangleLinesEx(rec, 5, BLUE);
}

#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_SAMPLE_SIZE 2
#define AUDIO_SAMPLE_CHANNEL 2
#define FPS 60
#define AUDIO_SAMPLE_FRAME_COUNT (AUDIO_SAMPLE_RATE / FPS)
#define AUDIO_SAMPLE_FRAME_BYTE (AUDIO_SAMPLE_RATE * AUDIO_SAMPLE_CHANNEL * AUDIO_SAMPLE_SIZE / FPS)
char silence[AUDIO_SAMPLE_FRAME_BYTE]; // 双声道 0.16
unsigned int wave_index = 0;

void plug_update(Env *env)
{
    BeginDrawing();
    show(env->dt, env->w, env->h);
    EndDrawing();

    memset(silence, 0, sizeof(silence));

    if(env->record)
    {
        Image screen = LoadImageFromScreen();
        fwrite(screen.data, screen.width * screen.height*4, 1, env->f_video);
        UnloadImage(screen);

        if(env->audio_index + AUDIO_SAMPLE_FRAME_COUNT < w.frameCount)
        {
            fwrite(w.data + env->audio_index*AUDIO_SAMPLE_SIZE*AUDIO_SAMPLE_CHANNEL, AUDIO_SAMPLE_FRAME_BYTE, 1, env->f_audio);
            env->audio_index += AUDIO_SAMPLE_FRAME_COUNT;
        }
        else if(env->audio_index < w.frameCount)
        {
            fwrite(w.data + env->audio_index*AUDIO_SAMPLE_SIZE*AUDIO_SAMPLE_CHANNEL, (w.frameCount - env->audio_index)*AUDIO_SAMPLE_SIZE*AUDIO_SAMPLE_CHANNEL, 1, env->f_audio);
            fwrite(silence, AUDIO_SAMPLE_FRAME_BYTE - (w.frameCount - env->audio_index)*AUDIO_SAMPLE_SIZE*AUDIO_SAMPLE_CHANNEL, 1, env->f_audio);
            env->audio_index += AUDIO_SAMPLE_FRAME_COUNT;
        }
        else
        {
            fwrite(silence, AUDIO_SAMPLE_FRAME_BYTE, 1, env->f_audio);
        }
    }
}

void plug_exit()
{
    unload_resources();
    free(p);
    system("ffmpeg -i output.mp4 -i output.wav -c:v copy -c:a copy out.mp4");
}