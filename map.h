#ifndef MAP_H
#define MAP_H

#include<vector>//用向量容器存放相邻顶点
using namespace std;


#define ROW 30//地图的宽
#define COL 30//地图的长
#define RODE 9//路宽
#define WALL 3//墙宽

enum DIR//枚举四个方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};


class Map
{
public:
	int val;//该顶点的标记
	int x;//该顶点在数组中位置
	int y;
	bool isEnd = false;
	vector<Map*>next;//存放相连顶点
	Map()
	{
		val = 0;
		x = 0;
		y = 0;
	}

	Map(int c, int r)
	{
		x = c;
		y = r;
	}

	void push(Map* m);

	void erase(Map* m);
};

#endif // MAP_H