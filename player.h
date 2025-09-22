#pragma once
#include <easyx.h>      // ֻ�õ� setfillcolor / solidcircle
#include "Map.h"        // ��� Map ��������ֻҪ������ neighbors ��������

extern Map* map[COL][ROW]; // ���ڱ𴦶����ȫ���Թ����飬����ֻ����

class Player
{
public:
    Player(int gx, int gy);     // �������������Ϊ������
    void move(int dx, int dy);  // dx,dy �� {-1,0,1}������ײ���
    void draw() const;          // �ڸ������Ļ�һ����ɫԲ
    bool reachEnd() const;      // �ж��Ƿ�վ�� isEnd==true �ĸ�����

    int x() const { return gridX; }
    int y() const { return gridY; }

private:
    int gridX, gridY;           // ��ǰ���ڸ����߼�����
    int pixX, pixY;            // ��Ӧ�������ģ����ڻ��ƣ�
    mutable int oldPixX, oldPixY;

    void syncPix();             // ���� gridX/gridY ˢ�� pixX/pixY
    bool canGo(int dx, int dy) const; // �ھ��б����Ƿ���Ŀ�����
};