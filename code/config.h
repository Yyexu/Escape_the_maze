#pragma once
#include <string>
#include <vector>

// ① 仍然保留“宏”默认值，保证 map.h 里的数组维度编译通过
#ifndef ROW
#define ROW 30
#endif
#ifndef COL
#define COL 30
#endif

// ② 真正的运行时常量（会在 initConfig() 里被重新赋值）
extern int g_ROW;
extern int g_COL;
extern int START_X;
extern int START_Y;
extern int END_X;
extern int END_Y;

// ③ 读入自定义墙图（1墙 0路）
std::vector<std::vector<bool>> loadCustomMap();

extern bool USE_CUSTOM_MAP;   // 1=用自定义图  0=随机迷宫
// ④ 在 main() 最开始调用一次
void initConfig();