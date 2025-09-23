#pragma once
#include <easyx.h>      // 只用到 setfillcolor / solidcircle
#include "Map.h"        // 你的 Map 类声明，只要里面有 neighbors 容器即可

extern Map* maze[COL][ROW];

class Player
{
public:
    Player(int gx, int gy);     // 传入格子坐标作为出生点
    int Steps = 0;
    void move(int dx, int dy);  // dx,dy ∈ {-1,0,1}，带碰撞检测
    int pixX, pixY;            // 对应像素中心（用于绘制）
    mutable int oldPixX, oldPixY;
    int gridX, gridY;           // 当前所在格子逻辑坐标
    int getgridX() { return gridX; };
    int getgridY() { return gridY; };
    bool reachEnd() const;      // 判断是否站在 isEnd==true 的格子上
    void draw() const;
    int x() const { return gridX; }
    int y() const { return gridY; }

private:
    void syncPix();             // 根据 gridX/gridY 刷新 pixX/pixY
    bool canGo(int dx, int dy) const; // 邻居列表里是否有目标格子
};