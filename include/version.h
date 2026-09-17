#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Version
{
    int major;
    int minor;
    int patch;
    char prerelease[32]; /* 如 "alpha.1" */
    char build[32];      /* 如 "20260830" */
} Version;

extern const Version version;
extern const char *const version_str;

static inline const char* version_get_string(const Version* version)
{
    static char buffer[128];

    snprintf(buffer, sizeof(buffer), "%d.%d.%d", version->major, version->minor, version->patch);

    if (version->prerelease[0] != '\0')
    {
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "-%s", version->prerelease);
        strncat(buffer, tmp, sizeof(buffer) - strlen(buffer) - 1);
    }

    if (version->build[0] != '\0')
    {
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "+%s", version->build);
        strncat(buffer, tmp, sizeof(buffer) - strlen(buffer) - 1);
    }

    return buffer;
}

/* 获取当前版本结构体 */
static inline const Version* version_get_current(void)
{
    return &version;
}

/* 将字符串转为小写（辅助函数） */
static inline void str_tolower(char* dst, const char* src, size_t n)
{
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++)
    {
        dst[i] = tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

/* 比较两个预发布标识符（如 "alpha" vs "beta"）
   返回值: 1 表示 a > b, -1 表示 a < b, 0 表示相等 */
static inline int compare_prerelease_identifiers(const char* a, const char* b)
{
    char a_lower[64], b_lower[64];
    str_tolower(a_lower, a, sizeof(a_lower));
    str_tolower(b_lower, b, sizeof(b_lower));
    return strcmp(a_lower, b_lower);
}

/* 主版本比较函数
   返回值: 1 表示 v1 > v2, -1 表示 v1 < v2, 0 表示相等 */
static inline int version_compare(const Version* v1, const Version* v2)
{
    /* 1. 比较数字部分 (major, minor, patch) */
    if (v1->major != v2->major)
        return (v1->major > v2->major) ? 1 : -1;
    if (v1->minor != v2->minor)
        return (v1->minor > v2->minor) ? 1 : -1;
    if (v1->patch != v2->patch)
        return (v1->patch > v2->patch) ? 1 : -1;

    /* 2. 数字部分相等，比较预发布版本 */
    int v1_has_pre = (v1->prerelease[0] != '\0');
    int v2_has_pre = (v2->prerelease[0] != '\0');

    /* 规则: 正式版(无pre) > 任何预发布版(有pre) */
    if (!v1_has_pre && v2_has_pre)
        return 1;
    if (v1_has_pre && !v2_has_pre)
        return -1;
    if (!v1_has_pre && !v2_has_pre)
        return 0; /* 完全相同 */

    /* 两者都有预发布标识，按字典序比较（如 alpha < beta < rc） */
    return compare_prerelease_identifiers(v1->prerelease, v2->prerelease);
}

/* 检查版本是否兼容 (v1 是否兼容 v2, 即主版本号相同且v1 >= v2) */
static inline int version_is_compatible(const Version* v1, const Version* v2)
{
    if (v1->major != v2->major)
        return 0; /* 主版本不同，不兼容 */
    return (version_compare(v1, v2) >= 0);
}