#!/usr/bin/env python3
"""
解析 impl.h，生成:
  platform_api.h  —— PlatformApi 结构体 + platform_get_api 声明
  platform.h      —— platform_xxx 函数声明
  platform.c      —— platform_xxx 实现 + PlatformApi 实例 + platform_get_api 实现

三个文件都 include 与 impl.h 相同的头文件 (prologue)。
"""

import re
import sys
import argparse

# ---------------------------------------------------------------
# 工具
# ---------------------------------------------------------------

def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    s = re.sub(r'//[^\n]*', ' ', s)
    return s

def find_matching(s, open_idx):
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '(':
            depth += 1
        elif s[i] == ')':
            depth -= 1
            if depth == 0:
                return i
    return -1

def split_top_commas(args):
    parts, depth, cur = [], 0, ''
    for c in args:
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        if c == ',' and depth == 0:
            parts.append(cur)
            cur = ''
        else:
            cur += c
    if cur.strip():
        parts.append(cur)
    return parts

def arg_names(args):
    """从参数列表提取参数名，用于转发"""
    a = args.strip()
    if a in ('', 'void'):
        return ''
    names = []
    for p in split_top_commas(a):
        p = p.strip()
        if not p:
            continue
        toks = re.findall(r'[A-Za-z_][A-Za-z0-9_]*', p)
        if toks:
            names.append(toks[-1])
    return ', '.join(names)

# ---------------------------------------------------------------
# 解析 impl.h
# ---------------------------------------------------------------

INCLUDE_RE = re.compile(r'^\s*#\s*(include|define)\b.*$', re.M)
PRAGMA_RE  = re.compile(r'^\s*#\s*pragma\b.*$', re.M)
FUNC_RE    = re.compile(
    r'([A-Za-z_][A-Za-z0-9_ \t\*]*?)\s+'      # 返回类型
    r'(impl_[A-Za-z_][A-Za-z0-9_]*)\s*'        # 名字
    r'\('                                      # 左括号
)

def parse_impl_h(path):
    raw = open(path, encoding='utf-8').read()
    src = strip_comments(raw)

    funcs = []
    seen = set()
    prologue_end = None

    for m in FUNC_RE.finditer(src):
        ret  = m.group(1).strip()
        name = m.group(2)

        if name in seen:
            continue

        # 名字前必须是标识符边界
        if m.start(2) > 0 and (src[m.start(2)-1].isalnum() or src[m.start(2)-1] == '_'):
            continue

        open_idx = m.end() - 1
        close_idx = find_matching(src, open_idx)
        if close_idx < 0:
            continue

        # 声明必须以 ';' 结尾 (跳过函数体实现)
        # r = close_idx + 1
        # while r < len(src) and src[r] in ' \t\n\r':
        #     r += 1
        # if r >= len(src) or src[r] != ';':
        #     continue

        # 去掉返回类型里的 static
        if ret.startswith('static'):
            ret = ret[len('static'):].strip()

        if ret.startswith('inline'):
            ret = ret[len('inline'):].strip()

        args = src[open_idx+1:close_idx].strip()
        if args == '' or args == 'void':
            args = 'void'

        funcs.append((ret, name, args))
        seen.add(name)

        if prologue_end is None:
            prologue_end = m.start()

    # prologue: 第一条函数声明之前的所有 include/define
    head = src[:prologue_end] if prologue_end is not None else src
    includes = []
    for line in head.splitlines():
        if PRAGMA_RE.match(line):
            continue
        if INCLUDE_RE.match(line):
            includes.append(line.rstrip())
        elif line.strip().startswith('#'):
            # 其他预处理指令也带上（如 #ifndef 条件宏），按需可去掉
            includes.append(line.rstrip())

    return includes, funcs

# ---------------------------------------------------------------
# 生成
# ---------------------------------------------------------------

BANNER = "/* 本文件由 gen_from_impl_h.py 自动生成, 请勿手动修改 */"

def gen_platform_h(includes, funcs):
    L = []
    L.append('#pragma once')
    L.append('')
    L.append(BANNER)
    L.append('')
    L.extend(includes)
    L.append('')
    for ret, name, args in funcs:
        base = name[5:]
        L.append(f'{ret} platform_{base}({args});')
    L.append('')
    L.append('typedef struct PlatformApi PlatformApi;')
    L.append('PlatformApi* platform_get_api(void);')
    return '\n'.join(L) + '\n'

def gen_platform_api_h(includes, funcs):
    L = []
    L.append('#pragma once')
    L.append('')
    L.append(BANNER)
    L.append('')
    # L.extend(includes)
    L.append('')
    L.append('typedef struct PlatformApi {')
    for ret, name, args in funcs:
        base = name[5:]
        L.append(f'    {ret} (*{base})({args});')
    L.append('} PlatformApi;')
    return '\n'.join(L) + '\n'

def gen_platform_plugin_h(includes, funcs):
    L = []
    L.append('#pragma once')
    L.append('')
    L.append(BANNER)
    L.append('')
    # L.extend(includes)
    L.append('#include "platform_def.h"')
    L.append('')
    L.append('typedef struct PlatformApi PlatformApi;')
    L.append('')
    for ret, name, args in funcs:
        base = name[5:]
        L.append(f'{ret} plugin_{base}({args});')
        L.append('')
    L.append('void plugin_set_api(PlatformApi *api);')
    return '\n'.join(L) + '\n'

def gen_platform_plugin_c(includes, funcs):
    L = []
    L.append(BANNER)
    L.append('')
    # L.extend(includes)
    L.append('#include "platform_plugin.h"')
    L.append('#include "platform_api.h"')
    L.append('')
    L.append('static PlatformApi *platform;')
    L.append('')
    for ret, name, args in funcs:
        base = name[5:]
        call = arg_names(args)
        L.append(f'{ret} plugin_{base}({args})')
        L.append('{')
        L.append(f'    return platform->{base}({call});')
        L.append('}')
        L.append('')
    L.append('void plugin_set_api(PlatformApi *api)')
    L.append('{')
    L.append('    platform = api;')
    L.append('}')
    return '\n'.join(L) + '\n'

def gen_platform_c(includes, funcs, impl_header='impl.h'):
    L = []
    L.append(BANNER)
    L.append('')
    # 先 include platform.h 和 platform_api.h, 再 include impl.h
    L.append('#include "platform.h"')
    L.append('#include "platform_api.h"')
    L.append(f'#include "{impl_header}"')
    L.append('')
    L.append('/* ---- platform_xxx 实现 ---- */')
    for ret, name, args in funcs:
        base = name[5:]
        call = arg_names(args)
        L.append(f'{ret} platform_{base}({args})')
        L.append('{')
        if ret == 'void':
            L.append(f'    {name}({call});')
        else:
            L.append(f'    return {name}({call});')
        L.append('}')
        L.append('')
    L.append('/* ---- PlatformApi 实例 ---- */')
    L.append('static PlatformApi platform = {')
    for ret, name, args in funcs:
        base = name[5:]
        L.append(f'    .{base} = platform_{base},')
    L.append('};')
    L.append('')
    L.append('PlatformApi* platform_get_api(void)')
    L.append('{')
    L.append('    return &platform;')
    L.append('}')
    L.append('')
    return '\n'.join(L)

# ---------------------------------------------------------------
# main
# ---------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('impl_h', help='impl.h 路径')
    ap.add_argument('--out-dir', default='.', help='输出目录')
    ap.add_argument('--impl-header', default='impl.h', help='platform.c 中 include 的 impl 头文件名')
    ap.add_argument('--platform-h', default='platform.h')
    ap.add_argument('--platform-api-h', default='platform_api.h')
    ap.add_argument('--platform-plugin-h', default='platform_plugin.h')
    ap.add_argument('--platform-plugin-c', default='platform_plugin.c')
    ap.add_argument('--platform-c', default='platform.c')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()

    includes, funcs = parse_impl_h(args.impl_h)

    if not funcs:
        print('未找到 impl_ 开头函数声明', file=sys.stderr)
        sys.exit(1)

    print(f'找到 {len(funcs)} 个接口，{len(includes)} 条前导 include', file=sys.stderr)
    for ret, name, a in funcs:
        print(f'  {ret} {name}({a})', file=sys.stderr)

    ph  = gen_platform_h(includes, funcs)
    pah = gen_platform_api_h(includes, funcs)
    pph = gen_platform_plugin_h(includes, funcs)
    ppc = gen_platform_plugin_c(includes, funcs)
    pc  = gen_platform_c(includes, funcs, args.impl_h)

    if args.dry_run:
        print('===== platform.h ====='); print(ph)
        print('===== platform_api.h ====='); print(pah)
        print('===== platform_plugin.h ====='); print(pph)
        print('===== platform_plugin.c ====='); print(ppc)
        print('===== platform.c ====='); print(pc)
        return

    import os
    def w(name, content):
        p = os.path.join(args.out_dir, name)
        open(p, 'w', encoding='utf-8').write(content)
        print(f'已写入 {p}', file=sys.stderr)

    w(args.platform_h, ph)
    w(args.platform_api_h, pah)
    w(args.platform_plugin_h, pph)
    w(args.platform_plugin_c, ppc)
    w(args.platform_c, pc)

if __name__ == '__main__':
    main()