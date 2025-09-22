#ifndef MAP_H
#define MAP_H

#include<vector>//����������������ڶ���
using namespace std;


#define ROW 30//��ͼ�Ŀ�
#define COL 30//��ͼ�ĳ�
#define RODE 9//·��
#define WALL 3//ǽ��

enum DIR//ö���ĸ�����
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};


class Map
{
public:
	int val;//�ö���ı��
	int x;//�ö�����������λ��
	int y;
	bool isEnd = false;
	vector<Map*>next;//�����������
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