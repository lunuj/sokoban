#pragma once
typedef struct PlatformPoint {
    int x;
    int y;
} PlatformPoint;

typedef struct PlatformSize {
    int width;
    int height;
} PlatformSize;

typedef struct PlatformColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} PlatformColor;

typedef struct PlatformRectangle {
    float x;
    float y;
    float width;
    float height;
} PlatformRectangle;

typedef struct PlatformWindowInfo {
    int width;
    int height;
    int render_width;
    int render_height;
    float dpi_scale;
    bool fullscreen;
    bool resizable;
} PlatformWindowInfo;

typedef enum PlatformLogLevel {
    PLATFORM_LOG_DEBUG = 0,
    PLATFORM_LOG_INFO  = 1,
    PLATFORM_LOG_WARN  = 2,
    PLATFORM_LOG_ERROR = 3,
    PLATFORM_LOG_FATAL = 4,
} PlatformLogLevel;

typedef struct PlatformServices PlatformServices;