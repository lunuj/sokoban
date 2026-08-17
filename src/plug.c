#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <assert.h> 
#include <math.h> 

#include "raylib/raylib.h"
#include "plug.h"
#include "anim.h"

typedef struct {
    size_t size;

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

Wave w;

void plug_init()
{
    p = malloc(sizeof(*p));
    assert(p != NULL);

    memset(p, 0, sizeof(*p));
    p->size = sizeof(*p);

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

Camera2D camera = {{155, 55}, {0, 0}, 0.0f, 1.0f};
Vector2 mp = {0, 0};
Vector2 p1 ={0, 0}, p2 = { 60, 0}, p3 = { 0, 60};
Animation animation[10];
AnimValue p1_to = {
    .type = ANIMATION_POSITION,
    .from.position = {0, 0},
    .to.position = {60, 60},
};
AnimValue p2_to = {
    .type = ANIMATION_POSITION,
    .from.position = {60, 0},
    .to.position = {0, 0},
};
AnimValue p3_to = {
    .type = ANIMATION_POSITION,
    .from.position = {0, 60},
    .to.position = {60, 0},
};
AnimValue p4_to = {
    .type = ANIMATION_POSITION,
    .from.position = {60, 60},
    .to.position = {0, 60},
};
AnimValue c1_to = {
    .type = ANIMATION_COLOR,
    .from.color = RED,
    .to.color = PINK,
};
Color c1 = RED;
Color c2 = BLUE;
Color c3 = GREEN;

Anim anim[16];
Sequence seq =  {
    .children = anim + 2,
    .count = 2,
};
Parallel para =  {
    .children = anim,
    .count = 2,
};

Parallel para2 =  {
    .children = anim + 5,
    .count = 2,
};

void show(float dt, float w, float h)
{
    (void)w;
    (void)h;

    int rw = 50;
    int rh = 50;

    BeginMode2D(camera);
    ClearBackground(BLANK);

    static int flag = 1;
    if(flag)
    {
        flag = !flag;
        animation_init(&animation[0], 0.5, p1_to, &p1);
        animation_init(&animation[1], 0.5, p2_to, &p2);
        animation_init(&animation[2], 0.5, p3_to, &p3);
        animation_init(&animation[3], 0.5, p4_to, &p1);
        animation_init(&animation[4], 0.5, c1_to, &c1);
    
        anim_init(&anim[0], ANIM_ANIMATION, animation);
        anim_init(&anim[1], ANIM_ANIMATION, animation+1);
        anim_init(&anim[2], ANIM_ANIMATION, animation+2);
        anim_init(&anim[3], ANIM_ANIMATION, animation+3);
        anim_init(&anim[4], ANIM_ANIMATION, animation+4);
        anim_init(&anim[5], ANIM_SEQUENCE, &seq);
        anim_init(&anim[6], ANIM_PARALLEL, &para);
        anim_init(&anim[7], ANIM_PARALLEL, &para2);
        anim[7].loop = 1;
    }

    // anim_update(&anim[5], dt);
    // anim_update(&anim[6], dt);
    anim_update(&anim[7], dt);

    DrawRectangle(p1.x, p1.y, rw, rh, c1);
    DrawRectangle(p2.x, p2.y, rw, rh, c2);
    DrawRectangle(p3.x, p3.y, rw, rh, c3);

    EndMode2D();
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
}