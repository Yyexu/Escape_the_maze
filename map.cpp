#include<iostream>
#include<graphics.h>//easyXͼ�ο�
#include<vector>//������ڶ���
#include<stack>//��ű���·��
#include<ctime>//�������

#include "map.h"

using namespace std;



void Map::push(Map* m) //������������
{
	next.push_back(m);
}

void Map::erase(Map* m) //�ж������ӵĶ���
{
	for (auto it = next.begin(); it != next.end(); it++)
	{
		if ((*it)->x == m->x && (*it)->y == m->y)
		{
			next.erase(it);
		}

	}
}
