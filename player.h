#pragma once
#include <easyx.h>      // 只用到 setfillcolor / solidcircle
#include "Map.h"        // 你的 Map 类声明，只要里面有 neighbors 容器即可

extern Map* map[COL][ROW]; // 你在别处定义的全局迷宫数组，这里只声明

class Player
{
public:
    Player(int gx, int gy);     // 传入格子坐标作为出生点
    void move(int dx, int dy);  // dx,dy ∈ {-1,0,1}，带碰撞检测
    void draw() const;          // 在格子中心画一个蓝色圆
    bool reachEnd() const;      // 判断是否站在 isEnd==true 的格子上

    int x() const { return gridX; }
    int y() const { return gridY; }

private:
    int gridX, gridY;           // 当前所在格子逻辑坐标
    int pixX, pixY;            // 对应像素中心（用于绘制）
    mutable int oldPixX, oldPixY;

    void syncPix();             // 根据 gridX/gridY 刷新 pixX/pixY
    bool canGo(int dx, int dy) const; // 邻居列表里是否有目标格子
};