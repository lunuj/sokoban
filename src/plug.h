#pragma once
#include <stdbool.h>

typedef struct {
    float dt;
    int w;
    int h;
    bool record;
    size_t audio_index;

    FILE* f_video;
    FILE* f_audio;
} Env;