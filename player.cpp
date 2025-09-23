#include "Player.h"
#include "map.h"

extern Map* map[COL][ROW];      // 与定义处链接

Player::Player(int gx, int gy) : gridX(gx), gridY(gy) {
    syncPix();
    oldPixX = pixX;
    oldPixY = pixY;
}

void Player::move(int dx, int dy)
{
    if (canGo(dx, dy)) {
        oldPixX = pixX;
        oldPixY = pixY;
        gridX += dx;
        gridY += dy;
        Steps++;
        syncPix();

    }
}

void Player::draw() const
{
    setfillcolor(WHITE);
    solidcircle(oldPixX, oldPixY, RODE / 2);

    setfillcolor(BLUE);
    solidcircle(pixX, pixY, RODE / 2);

    oldPixX = pixX;
    oldPixY = pixY;
}

bool Player::reachEnd() const
{
    return map[gridX][gridY]->isEnd;
}

/* ---------- 内部工具 ---------- */
void Player::syncPix()
{
    pixX = gridX * (RODE + WALL);
    pixY = gridY * (RODE + WALL);
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
