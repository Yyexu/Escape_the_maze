#include <iostream>
#include <graphics.h>//easyXͼ�ο�
#include <conio.h>
#include <vector>//����������������ڶ���
#include <stack>//�ö�ջ������ű���·��
#include <ctime>//Ҫʵ�����������Ҫ����һ����������ӣ���򵥵ı���ϵͳʱ��
#include <string>
#pragma comment(lib, "winmm.lib")
#include "map.h"
#include "game.h"

using namespace std;



Map* start = new Map;//�����ϽǶ�����Ϊ��һ������
Map* map[COL][ROW] = { 0 };//�������������Щ�����ָ��

void Game::init() {
	Game_Name = _T("�����Թ�");
	GAME_WIDTH = 1200;
	GAME_HEIGHT = 900;

	window = initgraph(GAME_WIDTH, GAME_HEIGHT);
	SetWindowText(window, Game_Name);
	//����̨Ȩ��
	hConsole = GetConsoleWindow();

	HICON hIcon = (HICON)LoadImage(
		NULL,                               // ����ΪNULL
		_T("res/pic/tb.ico"),               // ͼ���ļ�·��
		IMAGE_ICON,                         // ͼ������
		0, 0,                               // ʹ��Ĭ�ϴ�С
		LR_LOADFROMFILE | LR_DEFAULTSIZE    // ���ļ����ز�ʹ��Ĭ�ϴ�С
	);

	if (hIcon) {
		SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
	else {
		MessageBox(NULL, L"ͼ�����ʧ��!", L"����", MB_ICONERROR);
	}


	//����״̬�� state
	enum GameState state;
	state = MAIN_MENU;
	render(state);
}
//ˢ�»��� ����״̬��������ˢ��ѡ�����
void Game::render(enum GameState state) {

	switch (state) {
	//���˵�
	case MAIN_MENU:
		cleardevice();
		playSound(_T("res\\music\\menu.wav"), _T("bj"));

		cleardevice();
		loadimage(NULL, _T("res/pic/bg.jpg"), 1200, 900, true);
		DrawButton(GAME_WIDTH / 2, GAME_HEIGHT / 2 - 100, 200, 50, _T("��ʼ��Ϸ"));
		DrawButton(GAME_WIDTH / 2, GAME_HEIGHT / 2, 200, 50, _T("�Ѷ�����"));
		DrawButton(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 100, 200, 50, _T("Ƥ��ѡ��"));
		DrawButton(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 200, 200, 50, _T("��ͼѡ��"));
		break;

	//��Ϸ����
	case GAME_PLAY: {
		srand((unsigned)time(NULL));//���һ�����������ʹ���Թ������������
		setorigin(20, 20);//��������ԭ��
		setbkcolor(WHITE);//���ñ�����ɫ
		cleardevice();//��ձ�����ʹ���µı�����ɫ
		setfillcolor(BLACK);//�ú�ɫ���Թ���ǽ

		//�����ǰ����е�ǽ��������
		for (int i = -1; i < ROW; i++)
		{
			solidrectangle(-RODE / 2 - WALL, i * (RODE + WALL) + RODE / 2 + 1, (COL - 1) * (RODE + WALL) + RODE / 2 + WALL, i * (RODE + WALL) + RODE / 2 + WALL);
		}
		for (int i = -1; i < COL; i++)
		{
			solidrectangle(i * (RODE + WALL) + RODE / 2 + 1, -RODE / 2 - WALL, i * (RODE + WALL) + RODE / 2 + WALL, (ROW - 1) * (RODE + WALL) + RODE / 2 + WALL);
		}


		//ʵ����ÿһ������
		for (int i = 0; i < COL; i++)
		{
			for (int j = 0; j < ROW; j++)
			{
				map[i][j] = new Map(i, j);
			}
		}

		map[0][0] = start;//��start���뼯��

		setfillcolor(WHITE);
		//0 0 Ϊ��㣬���Ͻ�
		int x = 0, y = 0;
		stack<Map*> rode;//�ö�ջ�洢���ʹ��Ķ��㣬������ڵĶ���û�п����ӵľͰ�ԭ·����
		map[x][y]->val = 1;//��1������ѷ��ʹ��Ķ���
		rode.push(map[x][y]);

		while (1)
		{
			Sleep(1);//�����ӳٿ��Թ۲��Թ������Ĺ���
			int v = visit(x, y);
			if (v == 0)//û�п���ͨ�Ķ����ˣ���ԭ·����
			{
				rode.pop();
				if (rode.empty())//ջΪ�գ�˵��ȫ�����㶼���ʹ����Թ��ѹ������
					break;
				x = rode.top()->x;
				y = rode.top()->y;
			}
			else
			{//��������������ѡ��һ���ɷ��ʵĶ���
				vector<int>put;
				if (v % 10 == 1)//��visit�ķ���ֵ���зֽ�
					put.push_back(UP);
				if (v % 100 >= 10)
					put.push_back(DOWN);
				if (v % 1000 >= 100)
					put.push_back(LEFT);
				if (v >= 1000)
					put.push_back(RIGHT);

				int choose = rand() % put.size();
				//�ж���һ�������뵱ǰ��������λ�ã���������֮���ǽ��������ð�ɫ���δ��棬Ȼ��ı�����
				if (put[choose] == UP)
				{
					solidrectangle(x * (RODE + WALL) - RODE / 2, y * (RODE + WALL) - RODE / 2 - WALL, x * (RODE + WALL) + RODE / 2, y * (RODE + WALL) - RODE / 2);
					y--;
				}
				else if (put[choose] == DOWN)
				{
					solidrectangle(x * (RODE + WALL) - RODE / 2, y * (RODE + WALL) + RODE / 2, x * (RODE + WALL) + RODE / 2, y * (RODE + WALL) + RODE / 2 + WALL);
					y++;
				}
				else if (put[choose] == LEFT)
				{
					solidrectangle(x * (RODE + WALL) - RODE / 2 - WALL, y * (RODE + WALL) - RODE / 2, x * (RODE + WALL) - RODE / 2, y * (RODE + WALL) + RODE / 2);
					x--;
				}
				else if (put[choose] == RIGHT)
				{
					solidrectangle(x * (RODE + WALL) + RODE / 2, y * (RODE + WALL) - RODE / 2, x * (RODE + WALL) + RODE / 2 + WALL, y * (RODE + WALL) + RODE / 2);
					x++;
				}

				rode.top()->push(map[x][y]);//����һ��������뵱ǰ�����������
				map[x][y]->push(rode.top());//����ǰ���������һ�������������
				rode.push(map[x][y]);//������һ��������ջ
				map[x][y]->val = 1;//�ѷ��ʹ������Ϊ1
			}
		}

		player = new Player(0, 0);
		player->draw();

		map[COL - 1][ROW - 1]->isEnd = true;

		for (int i = 0; i < COL; i++) {
			for (int j = 0; j < ROW; j++) {
				if (map[i][j]->isEnd) {
					setfillcolor(GREEN);
					solidrectangle(
						i * (RODE + WALL) - RODE / 2,
						j * (RODE + WALL) - RODE / 2,
						i * (RODE + WALL) + RODE / 2,
						j * (RODE + WALL) + RODE / 2
					);
				}
			}
		}
		//��������߼�
		while (1) {
			ExMessage msg;
			while (peekmessage(&msg, EX_KEY | EX_MOUSE)) {
				if (msg.message == WM_KEYDOWN && !(msg.lParam & 0x40000000)) {
					int dx = 0, dy = 0;
					switch (msg.vkcode) {
					case VK_UP:    dy = -1; break;
					case VK_DOWN:  dy = 1; break;
					case VK_LEFT:  dx = -1; break;
					case VK_RIGHT: dx = 1; break;
					case VK_ESCAPE:
						if (MessageBox(this->window, _T("�Ƿ��˳���Ϸ"), _T("�˳���Ϸ"), MB_YESNO) == IDYES) {
							playSound(_T("res\\music\\click.wav"), _T("click"));
							Sleep(100);
							closegraph();
							if (hConsole)          // ��������Ч
								PostMessage(hConsole, WM_CLOSE, 0, 0);
						}
						break;
					}
					if (dx != 0 || dy != 0) {
						player->move(dx, dy);
					}
				}
			}

			player->draw();
			FlushBatchDraw();
			if (player->reachEnd()) {
				printf("���յ㿩");
				closegraph();
			}
			Sleep(80);     
		}
		break; 
	}

	case EXIT:
		if (MessageBox(this->window, _T("�Ƿ��˳���Ϸ"), _T("�˳���Ϸ"), MB_YESNO) == IDYES) {
			playSound(_T("res\\music\\click.wav"), _T("click"));
			Sleep(100);
			closegraph();
			if (hConsole)          // ��������Ч
				PostMessage(hConsole, WM_CLOSE, 0, 0);
		}
		break; 
	}
}


int Game::visit(int x, int y)
{
	int v = 0;//�ֱ��ø�λ��ʮλ����λ��ǧλ��ʾ�������ҿ���ͨ�Ŀ���
	//�����һ��λ�ò��Ǳ߽���߲����ѷ��ʹ��Ķ��㣬�ͱ�ʾ������ͨ
	if (y > 0 && map[x][y - 1]->val != 1)
	{
		v += 1;
	}
	if (y < ROW - 1 && map[x][y + 1]->val != 1)
	{
		v += 10;
	}
	if (x > 0 && map[x - 1][y]->val != 1)
	{
		v += 100;
	}
	if (x < COL - 1 && map[x + 1][y]->val != 1)
	{
		v += 1000;
	}
	return v;
}

bool Game::confirmExit()
{
	int ret = MessageBox(GetHWnd(),
		L"ȷ��Ҫ�˳���Ϸ��",
		L"��ʾ",
		MB_YESNO | MB_ICONQUESTION);
	return ret == IDYES;   // ֻ�е㡰�ǡ������˳�
}





void Game::DrawButton(int x, int y, int width, int height, LPCTSTR text) {
	setfillcolor(RGB(200, 200, 200));
	solidroundrect(x - width / 2, y - height / 2, x + width / 2, y + height / 2, 8, 8);

	setlinecolor(RGB(100, 100, 100));
	setlinestyle(PS_SOLID, 2);
	roundrect(x - width / 2, y - height / 2, x + width / 2, y + height / 2, 8, 8);

	settextcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	settextstyle(32, 0, _T("΢���ź�"));

	int textWidth = textwidth(text);
	int textHeight = textheight(text);
	int textX = x - textWidth / 2;
	int textY = y - textHeight / 2;
	outtextxy(textX, textY, text);
}

//��Ч����
void Game::playSound(LPCTSTR file, LPCTSTR alias) {
	TCHAR command[256];
	_stprintf_s(command, _T("close %s"), alias);
	mciSendString(command, NULL, 0, NULL);
	_stprintf_s(command, _T("open \"%s\" type waveaudio alias %s"), file, alias);
	mciSendString(command, NULL, 0, NULL);
	_stprintf_s(command, _T("play %s"), alias);
	mciSendString(command, NULL, 0, NULL);
}

void Game::stopSound(LPCTSTR alias) {
	TCHAR command[256];
	_stprintf_s(command, _T("close %s"), alias);
	mciSendString(command, NULL, 0, NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void findRode()
{
	int x = 0, y = 0;
	stack<Map*>rode;
	rode.push(start);
	map[x][y]->val = 2;//�����ʹ��Ķ�����Ϊ2
	setfillcolor(BLUE);//�����ʹ��Ķ���Ⱦ����ɫ
	solidrectangle(x * (RODE + WALL) - RODE / 2, y * (RODE + WALL) - RODE / 2, x * (RODE + WALL) + RODE / 2, y * (RODE + WALL) + RODE / 2);

	while (x != COL - 1 || y != ROW - 1)//�����յ㣬�������
	{
		Sleep(1);
		bool flag = 0;
		for (auto it = rode.top()->next.begin(); it != rode.top()->next.end(); it++)
		{//�Ե�ǰ������������б������ж��Ƿ��з��������������ڵ���Է���
			if (map[(*it)->x][(*it)->y]->val != 2)
			{
				rode.push(*it);
				x = (*it)->x;
				y = (*it)->y;
				map[x][y]->val = 2;
				solidrectangle(x * (RODE + WALL) - RODE / 2, y * (RODE + WALL) - RODE / 2, x * (RODE + WALL) + RODE / 2, y * (RODE + WALL) + RODE / 2);
				flag = 1;
				break;
			}
		}

		if (flag == 0)//���û�з��������Ķ��㣬�Ͱ�ԭ·����
		{
			rode.pop();
			x = rode.top()->x;
			y = rode.top()->y;
		}
	}

	setfillcolor(GREEN);//����ȷ��·��Ⱦ��
	while (!rode.empty())
	{
		x = rode.top()->x;
		y = rode.top()->y;
		rode.pop();
		solidrectangle(x * (RODE + WALL) - RODE / 2, y * (RODE + WALL) - RODE / 2, x * (RODE + WALL) + RODE / 2, y * (RODE + WALL) + RODE / 2);
	}
}
