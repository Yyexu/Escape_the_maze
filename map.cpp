#include<iostream>
#include<graphics.h>//easyX图形库
#include<vector>//存放相邻顶点
#include<stack>//存放遍历路径
#include<ctime>//用于随机

#include "map.h"

using namespace std;



void Map::push(Map* m) //连接其他顶点
{
	next.push_back(m);
}

void Map::erase(Map* m) //切断已连接的顶点
{
	for (auto it = next.begin(); it != next.end(); it++)
	{
		if ((*it)->x == m->x && (*it)->y == m->y)
		{
			next.erase(it);
		}

	}
}
