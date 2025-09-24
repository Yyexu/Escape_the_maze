#include <iostream>
#include <graphics.h>//easyXͼ�ο�
#include <conio.h>
#include <vector>//����������������ڶ���
#include <stack>//�ö�ջ������ű���·��
#include <ctime>//Ҫʵ�����������Ҫ����һ����������ӣ���򵥵ı���ϵͳʱ��
#include <string>
#include <time.h>//��Ҫͷ�ļ�
#include <queue>
#pragma comment(lib, "winmm.lib")
#include "map.h"
#include "game.h"
#include "player.h"
#include "config.h"

using namespace std;

Map* start = new Map;//�����ϽǶ�����Ϊ��һ������
Map* map[COL][ROW] = { 0 };//�������������Щ�����ָ��
std::stack<Map*> rode;




void Game::init() {
	Game_Name = _T("�����Թ�");
	GAME_WIDTH = 1200;
	GAME_HEIGHT = 900;
	initConfig();

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
		// ���˵�
	case MAIN_MENU:
		cleardevice();
		playSound(_T("res\\music\\menu.wav"), _T("bj"));

		cleardevice();
		loadimage(NULL, _T("res/pic/bg.jpg"), 1200, 900, true);
		DrawButton(GAME_WIDTH / 2, GAME_HEIGHT / 2 , 200, 50, _T("��ʼ��Ϸ"));
		break;

		// ��Ϸ����
	case GAME_PLAY: {
		int Steps = 0;
		srand((unsigned)time(NULL));// ���һ�����������ʹ���Թ������������
		setorigin(20, 20);// ��������ԭ��
		setbkcolor(WHITE);// ���ñ�����ɫ
		cleardevice();// ��ձ�����ʹ���µı�����ɫ
		setfillcolor(BLACK);// �ú�ɫ���Թ���ǽ

		// ==========================================================
		// ��  �Զ���ͼ��֧����ȫ�滻������ɣ�
		// ==========================================================
		if (USE_CUSTOM_MAP) {
			auto wall = loadCustomMap();
			if (wall.empty()) {
				MessageBox(window, L"[CustomMap] ��ȱʧ���ļ�����", L"��ʾ", MB_OK);
				USE_CUSTOM_MAP = false;   // ���˵����
			}
			else {
				// ----------- 1. ֻ����ǽ������ -----------
				for (int j = 0; j < g_ROW; ++j)
					for (int i = 0; i < g_COL; ++i) {
						map[i][j] = new Map(i, j);
						if (wall[j][i]) {          // 1=ǽ
							solidrectangle(i * (RODE + WALL) - RODE / 2,
								j * (RODE + WALL) - RODE / 2,
								i * (RODE + WALL) + RODE / 2,
								j * (RODE + WALL) + RODE / 2);
						}
					}
				// ----------- 2. ֻ����·���������� -----------
				for (int j = 0; j < g_ROW; ++j)
					for (int i = 0; i < g_COL; ++i) {
						if (wall[j][i]) continue;   // ǽ�����ڽ�
						if (j > 0 && !wall[j - 1][i]) map[i][j]->push(map[i][j - 1]);
						if (j < g_ROW - 1 && !wall[j + 1][i]) map[i][j]->push(map[i][j + 1]);
						if (i > 0 && !wall[j][i - 1]) map[i][j]->push(map[i - 1][j]);
						if (i < g_COL - 1 && !wall[j][i + 1]) map[i][j]->push(map[i + 1][j]);
					}
				// ----------- 3. �������ѷ��� --------
				map[START_X][START_Y]->val = 1;
			}
		}

		// ==========================================================
		// ��  ԭ�ⲻ��������ԭ�����������
		// ==========================================================
		if (!USE_CUSTOM_MAP) {
			// �����ǰ����е�ǽ��������
			for (int i = -1; i < g_ROW; i++)
			{
				solidrectangle(-RODE / 2 - WALL, i * (RODE + WALL) + RODE / 2 + 1, (g_COL - 1) * (RODE + WALL) + RODE / 2 + WALL, i * (RODE + WALL) + RODE / 2 + WALL);
			}
			for (int i = -1; i < g_COL; i++)
			{
				solidrectangle(i * (RODE + WALL) + RODE / 2 + 1, -RODE / 2 - WALL, i * (RODE + WALL) + RODE / 2 + WALL, (g_ROW - 1) * (RODE + WALL) + RODE / 2 + WALL);
			}

			// ʵ����ÿһ������
			for (int i = 0; i < COL; i++)
			{
				for (int j = 0; j < g_ROW; j++)
				{
					map[i][j] = new Map(i, j);
				}
			}

			map[0][0] = start;// �� start ���뼯��

			setfillcolor(WHITE);
			// 0 0 Ϊ��㣬���Ͻ�
			int x = 0, y = 0;
			stack<Map*> rode;// �ö�ջ�洢���ʹ��Ķ��㣬������ڵĶ���û�п����ӵľͰ�ԭ·����
			map[x][y]->val = 1;// �� 1 ������ѷ��ʹ��Ķ���
			rode.push(map[x][y]);

			while (1)
			{
				Sleep(1);// �����ӳٿ��Թ۲��Թ������Ĺ���
				int v = visit(x, y);
				if (v == 0)// û�п���ͨ�Ķ����ˣ���ԭ·����
				{
					rode.pop();
					if (rode.empty())// ջΪ�գ�˵��ȫ�����㶼���ʹ����Թ��ѹ������
						break;
					x = rode.top()->x;
					y = rode.top()->y;
				}
				else
				{// ��������������ѡ��һ���ɷ��ʵĶ���
					vector<int>put;
					if (v % 10 == 1)// �� visit �ķ���ֵ���зֽ�
						put.push_back(UP);
					if (v % 100 >= 10)
						put.push_back(DOWN);
					if (v % 1000 >= 100)
						put.push_back(LEFT);
					if (v >= 1000)
						put.push_back(RIGHT);

					int choose = rand() % put.size();
					// �ж���һ�������뵱ǰ��������λ�ã���������֮���ǽ��������ð�ɫ���δ��棬Ȼ��ı�����
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

					rode.top()->push(map[x][y]);// ����һ��������뵱ǰ�����������
					map[x][y]->push(rode.top());// ����ǰ���������һ�������������
					rode.push(map[x][y]);// ������һ��������ջ
					map[x][y]->val = 1;// �ѷ��ʹ������Ϊ 1
				}
			}
		}

		// ��ť��
		DrawButton(800, 400, 200, 50, _T("���������Թ�"));
		DrawButton(800, 600, 200, 50, _T("DFSѰ·"));
		DrawButton(800, 800, 200, 50, _T("BFS·"));

		player = new Player(START_X, START_Y);
		player->draw();

		map[END_X][END_Y]->isEnd = true;

		for (int i = 0; i < g_COL; i++) {
			for (int j = 0; j < g_ROW; j++) {
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

		bool isFinding = false;

		clock_t start = clock();

		// ��������߼�
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
							if (hConsole) PostMessage(hConsole, WM_CLOSE, 0, 0);
							return;
						}
						break;
					}
					if (dx != 0 || dy != 0) {
						player->move(dx, dy);
					}
				}

				if (msg.message == WM_LBUTTONDOWN) {
					if (msg.x >= 720 && msg.x <= 920 && msg.y >= 400 && msg.y <= 450) {
						playSound(_T("res\\music\\click.wav"), _T("click"));
						render(state);
					}
					if (msg.x >= 720 && msg.x <= 920 && msg.y >= 600 && msg.y <= 650) {
						playSound(_T("res\\music\\click.wav"), _T("click"));
						startFindRoad();   // ��ʼ��Ѱ·
						isFinding = true;  // ��ʼѰ·
						clock_t dfs = clock();
						dfsInProgress = false;
						bfsInProgress = true;
					}
					if (msg.x >= 720 && msg.x <= 920 && msg.y >= 800 && msg.y <= 850) {
						playSound(_T("res\\music\\click.wav"), _T("click"));
						startBFS();   // ��ʼ��Ѱ·
						isFinding = true;  // ��ʼѰ·
						clock_t bfs = clock();
						bfsInProgress = false;
						dfsInProgress = true;
					}
				}
			}

			if (!dfsInProgress && isFinding) {
				dfsInProgress = findRoadDFS(); // findRoadDFS����true��ʾ���
				if (dfsInProgress) {
					isFinding = false;
					clock_t dfs_end = (clock() - start) / CLOCKS_PER_SEC;
					printf("DFSѰ·��ʱ %d ��\n", dfs_end);
				}
			}

			if (!bfsInProgress && isFinding) {
				bfsInProgress = findRoadBFS();     // stepBFS����true��ʾ���
				if (bfsInProgress) {
					isFinding = false;
					clock_t bfs_end = (clock() - start) / CLOCKS_PER_SEC;
					printf("BFSѰ·��ʱ %d ��\n", bfs_end);
				}
			}

			// �ִ��յ�Ч��
			if (player->reachEnd()) {
				clock_t end = (clock() - start) / CLOCKS_PER_SEC;
				playSound(_T("res\\music\\tg.wav"), _T("tg"));
				printf("���յ㿩,һ������ %d ��,һ������ %d ��", player->Steps, end);
				Sleep(5000);
				closegraph();
			}

			player->draw();
			FlushBatchDraw();
			Sleep(16);
		}
		break;
	}
	case EXIT:
		if (MessageBox(this->window, _T("�Ƿ��˳���Ϸ"), _T("�˳���Ϸ"), MB_YESNO) == IDYES) {
			playSound(_T("res\\music\\click.wav"), _T("click"));
			Sleep(100);
			closegraph();
			if (hConsole)
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
	if (y < g_ROW -1 && map[x][y + 1]->val != 1)
	{
		v += 10;
	}
	if (x > 0 && map[x - 1][y]->val != 1)
	{
		v += 100;
	}
	if (x < g_COL-1 && map[x + 1][y]->val != 1)
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

bool Game::findRoadDFS() {
	if (rode.empty()) return true;

	Map* cur = rode.top();
	int x = cur->x;
	int y = cur->y;

	// �ִ��յ�
	if (x == END_X && y == END_Y) {
		while (!rode.empty()) {
			Map* p = rode.top();
			rode.pop();
			setfillcolor(GREEN);
			solidrectangle(
				p->x * (RODE + WALL) - RODE / 2,
				p->y * (RODE + WALL) - RODE / 2,
				p->x * (RODE + WALL) + RODE / 2,
				p->y * (RODE + WALL) + RODE / 2
			);
		}
		return true;
	}

	// DFS ��һ��
	for (auto it : cur->next) {
		if (map[it->x][it->y]->val != 2) {
			rode.push(it);
			map[it->x][it->y]->val = 2;
			setfillcolor(BLUE);
			solidrectangle(
				it->x * (RODE + WALL) - RODE / 2,
				it->y * (RODE + WALL) - RODE / 2,
				it->x * (RODE + WALL) + RODE / 2,
				it->y * (RODE + WALL) + RODE / 2
			);
			return false;
		}
	}

	rode.pop(); // ����
	return false;
}


// BFS Ѱ·
// Game.cpp

// ��ʼ�� BFS
void Game::startBFS() {
	// ���֮ǰ������
	while (!bfsQueue.empty()) bfsQueue.pop();
	memset(bfsParent, 0, sizeof(bfsParent));

	int px = player->getgridX();
	int py = player->getgridY();

	bfsQueue.push(map[px][py]);
	map[px][py]->val = 2; // ����ѷ���

}

// BFS һ��
bool Game::findRoadBFS() {
	if (bfsQueue.empty()) {
		bfsInProgress = false;
		return true; // ��������
	}

	Map* cur = bfsQueue.front();
	bfsQueue.pop();
	int x = cur->x;
	int y = cur->y;

	// �����յ�
	if (x == END_X && y == END_Y) {
		// ����·������
		Map* p = cur;
		while (p) {
			setfillcolor(GREEN);
			solidrectangle(
				p->x * (RODE + WALL) - RODE / 2,
				p->y * (RODE + WALL) - RODE / 2,
				p->x * (RODE + WALL) + RODE / 2,
				p->y * (RODE + WALL) + RODE / 2
			);
			p = bfsParent[p->x][p->y];
		}
		return true;
	}

	// �����ھ�
	for (auto it : cur->next) {
		if (map[it->x][it->y]->val != 2) {
			bfsQueue.push(it);
			map[it->x][it->y]->val = 2;
			bfsParent[it->x][it->y] = cur;

			// ����̽���ۼ�
			setfillcolor(BLUE);
			solidrectangle(
				it->x * (RODE + WALL) - RODE / 2,
				it->y * (RODE + WALL) - RODE / 2,
				it->x * (RODE + WALL) + RODE / 2,
				it->y * (RODE + WALL) + RODE / 2
			);
		}
	}

	return false; // BFS ��û����
}




void Game::startFindRoad() {
	while (!rode.empty()) rode.pop(); // ��վ�ջ

	int px = player->getgridX();  // ��ҵ�ǰ����
	int py = player->getgridY();

	rode.push(map[px][py]);       // ���λ����ջ
	map[px][py]->val = 2;         // ����߹�

}