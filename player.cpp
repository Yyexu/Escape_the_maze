#include "Player.h"
#include "map.h"

extern Map* map[COL][ROW];      // �붨�崦����

Player::Player(int gx, int gy) : gridX(gx), gridY(gy) { 
    syncPix();
    oldPixX = pixX;          // 2. ��ʼ������=��ǰ����
    oldPixY = pixY;
}

void Player::move(int dx, int dy)
{
    if (canGo(dx, dy)) {
        oldPixX = pixX;          // 3. ���浱ǰ֡��λ��
        oldPixY = pixY;
        gridX += dx;
        gridY += dy;
        syncPix();
    }
}

void Player::draw() const
{
    setfillcolor(WHITE);                     // ����ɫ
    solidcircle(oldPixX, oldPixY, RODE / 2);   // ������Բ

    setfillcolor(BLUE);                      // ���ɫ
    solidcircle(pixX, pixY, RODE / 2);      // ����Բ

    oldPixX = pixX;   // 4. ���¾����꣨����һ֡������
    oldPixY = pixY;
}

bool Player::reachEnd() const
{
    return map[gridX][gridY]->isEnd;   // ��Ҫ���� Map ����� bool isEnd=false;
}

/* ---------- �ڲ����� ---------- */
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