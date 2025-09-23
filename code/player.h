#pragma once
#include <easyx.h>      // ֻ�õ� setfillcolor / solidcircle
#include "Map.h"        // ��� Map ��������ֻҪ������ neighbors ��������

extern Map* maze[COL][ROW];

class Player
{
public:
    Player(int gx, int gy);     // �������������Ϊ������
    int Steps = 0;
    void move(int dx, int dy);  // dx,dy �� {-1,0,1}������ײ���
    int pixX, pixY;            // ��Ӧ�������ģ����ڻ��ƣ�
    mutable int oldPixX, oldPixY;
    int gridX, gridY;           // ��ǰ���ڸ����߼�����
    int getgridX() { return gridX; };
    int getgridY() { return gridY; };
    bool reachEnd() const;      // �ж��Ƿ�վ�� isEnd==true �ĸ�����
    void draw() const;
    int x() const { return gridX; }
    int y() const { return gridY; }

private:
    void syncPix();             // ���� gridX/gridY ˢ�� pixX/pixY
    bool canGo(int dx, int dy) const; // �ھ��б����Ƿ���Ŀ�����
};