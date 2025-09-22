#include "Player.h"
#include "map.h"

extern Map* map[COL][ROW];      // 与定义处链接

Player::Player(int gx, int gy) : gridX(gx), gridY(gy) { 
    syncPix();
    oldPixX = pixX;          // 2. 初始旧坐标=当前坐标
    oldPixY = pixY;
}

void Player::move(int dx, int dy)
{
    if (canGo(dx, dy)) {
        oldPixX = pixX;          // 3. 保存当前帧的位置
        oldPixY = pixY;
        gridX += dx;
        gridY += dy;
        syncPix();
    }
}

void Player::draw() const
{
    setfillcolor(WHITE);                     // 背景色
    solidcircle(oldPixX, oldPixY, RODE / 2);   // 擦掉旧圆

    setfillcolor(BLUE);                      // 玩家色
    solidcircle(pixX, pixY, RODE / 2);      // 画新圆

    oldPixX = pixX;   // 4. 更新旧坐标（供下一帧擦除）
    oldPixY = pixY;
}

bool Player::reachEnd() const
{
    return map[gridX][gridY]->isEnd;   // 需要你在 Map 里加了 bool isEnd=false;
}

/* ---------- 内部工具 ---------- */
void Player::syncPix()
{
    pixX = gridX * (RODE + WALL) ;
    pixY = gridY * (RODE + WALL) ;
}

bool Player::canGo(int dx, int dy) const
{
    int nx = gridX + dx;
    int ny = gridY + dy;
    if (nx < 0 || nx >= COL || ny < 0 || ny >= ROW) return false;

    Map* from = map[gridX][gridY];
    Map* to = map[nx][ny];
    for (Map* nei : from->next)      
        if (nei == to) return true;
    return false;
}