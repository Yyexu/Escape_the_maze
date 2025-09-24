#include <iostream>
#include <graphics.h>//easyX图形库
#include <conio.h>
#include <vector>//用向量容器存放相邻顶点
#include <stack>//用堆栈容器存放遍历路径
#include <ctime>//要实现真随机，需要引入一个随机数种子，最简单的便是系统时间
#include <string>
#include <time.h>//需要头文件
#include <queue>
#pragma comment(lib, "winmm.lib")
#include "map.h"
#include "game.h"
#include "player.h"
#include "config.h"

using namespace std;

Map* start = new Map;//将左上角顶点作为第一个顶点
Map* map[COL][ROW] = { 0 };//用数组来存放这些顶点的指针
std::stack<Map*> rode;




void Game::init() {
	Game_Name = _T("逃离迷宫");
	GAME_WIDTH = 1200;
	GAME_HEIGHT = 900;
	initConfig();

	window = initgraph(GAME_WIDTH, GAME_HEIGHT);
	SetWindowText(window, Game_Name);
	//控制台权柄
	hConsole = GetConsoleWindow();

	HICON hIcon = (HICON)LoadImage(
		NULL,                               // 必须为NULL
		_T("res/pic/tb.ico"),               // 图标文件路径
		IMAGE_ICON,                         // 图标类型
		0, 0,                               // 使用默认大小
		LR_LOADFROMFILE | LR_DEFAULTSIZE    // 从文件加载并使用默认大小
	);

	if (hIcon) {
		SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
	else {
		MessageBox(NULL, L"图标加载失败!", L"错误", MB_ICONERROR);
	}


	//定义状态机 state
	enum GameState state;
	state = MAIN_MENU;
	render(state);
}
//刷新机制 根据状态机来进行刷新选择绘制
void Game::render(enum GameState state) {

	switch (state) {
		// 主菜单
	case MAIN_MENU:
		cleardevice();
		playSound(_T("res\\music\\menu.wav"), _T("bj"));

		cleardevice();
		loadimage(NULL, _T("res/pic/bg.jpg"), 1200, 900, true);
		DrawButton(GAME_WIDTH / 2, GAME_HEIGHT / 2 , 200, 50, _T("开始游戏"));
		break;

		// 游戏界面
	case GAME_PLAY: {
		int Steps = 0;
		srand((unsigned)time(NULL));// 添加一个随机数种子使得迷宫的生成随机化
		setorigin(20, 20);// 设置坐标原点
		setbkcolor(WHITE);// 设置背景颜色
		cleardevice();// 清空背景以使用新的背景颜色
		setfillcolor(BLACK);// 用黑色画迷宫的墙

		// ==========================================================
		// ①  自定义图分支（完全替换随机生成）
		// ==========================================================
		if (USE_CUSTOM_MAP) {
			auto wall = loadCustomMap();
			if (wall.empty()) {
				MessageBox(window, L"[CustomMap] 段缺失或文件错误！", L"提示", MB_OK);
				USE_CUSTOM_MAP = false;   // 回退到随机
			}
			else {
				// ----------- 1. 只画“墙”方块 -----------
				for (int j = 0; j < g_ROW; ++j)
					for (int i = 0; i < g_COL; ++i) {
						map[i][j] = new Map(i, j);
						if (wall[j][i]) {          // 1=墙
							solidrectangle(i * (RODE + WALL) - RODE / 2,
								j * (RODE + WALL) - RODE / 2,
								i * (RODE + WALL) + RODE / 2,
								j * (RODE + WALL) + RODE / 2);
						}
					}
				// ----------- 2. 只给“路”四向连边 -----------
				for (int j = 0; j < g_ROW; ++j)
					for (int i = 0; i < g_COL; ++i) {
						if (wall[j][i]) continue;   // 墙→无邻接
						if (j > 0 && !wall[j - 1][i]) map[i][j]->push(map[i][j - 1]);
						if (j < g_ROW - 1 && !wall[j + 1][i]) map[i][j]->push(map[i][j + 1]);
						if (i > 0 && !wall[j][i - 1]) map[i][j]->push(map[i - 1][j]);
						if (i < g_COL - 1 && !wall[j][i + 1]) map[i][j]->push(map[i + 1][j]);
					}
				// ----------- 3. 标记起点已访问 --------
				map[START_X][START_Y]->val = 1;
			}
		}

		// ==========================================================
		// ②  原封不动保留你原来的随机生成
		// ==========================================================
		if (!USE_CUSTOM_MAP) {
			// 下面是把所有的墙都画出来
			for (int i = -1; i < g_ROW; i++)
			{
				solidrectangle(-RODE / 2 - WALL, i * (RODE + WALL) + RODE / 2 + 1, (g_COL - 1) * (RODE + WALL) + RODE / 2 + WALL, i * (RODE + WALL) + RODE / 2 + WALL);
			}
			for (int i = -1; i < g_COL; i++)
			{
				solidrectangle(i * (RODE + WALL) + RODE / 2 + 1, -RODE / 2 - WALL, i * (RODE + WALL) + RODE / 2 + WALL, (g_ROW - 1) * (RODE + WALL) + RODE / 2 + WALL);
			}

			// 实例化每一个顶点
			for (int i = 0; i < COL; i++)
			{
				for (int j = 0; j < g_ROW; j++)
				{
					map[i][j] = new Map(i, j);
				}
			}

			map[0][0] = start;// 把 start 加入集合

			setfillcolor(WHITE);
			// 0 0 为起点，左上角
			int x = 0, y = 0;
			stack<Map*> rode;// 用堆栈存储访问过的顶点，如果相邻的顶点没有可连接的就按原路返回
			map[x][y]->val = 1;// 用 1 来标记已访问过的顶点
			rode.push(map[x][y]);

			while (1)
			{
				Sleep(1);// 设置延迟可以观察迷宫构建的过程
				int v = visit(x, y);
				if (v == 0)// 没有可连通的顶点了，按原路返回
				{
					rode.pop();
					if (rode.empty())// 栈为空，说明全部顶点都访问过，迷宫已构造完成
						break;
					x = rode.top()->x;
					y = rode.top()->y;
				}
				else
				{// 下面代码用来随机选择一个可访问的顶点
					vector<int>put;
					if (v % 10 == 1)// 对 visit 的返回值进行分解
						put.push_back(UP);
					if (v % 100 >= 10)
						put.push_back(DOWN);
					if (v % 1000 >= 100)
						put.push_back(LEFT);
					if (v >= 1000)
						put.push_back(RIGHT);

					int choose = rand() % put.size();
					// 判断下一个顶点与当前顶点的相对位置，并将它们之间的墙拆掉，即用白色矩形代替，然后改变坐标
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

					rode.top()->push(map[x][y]);// 将下一个顶点加入当前顶点的容器中
					map[x][y]->push(rode.top());// 将当前顶点加入下一个顶点的容器中
					rode.push(map[x][y]);// 并将下一个顶点入栈
					map[x][y]->val = 1;// 已访问过，标记为 1
				}
			}
		}

		// 按钮们
		DrawButton(800, 400, 200, 50, _T("重新生成迷宫"));
		DrawButton(800, 600, 200, 50, _T("DFS寻路"));
		DrawButton(800, 800, 200, 50, _T("BFS路"));

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

		// 玩家行走逻辑
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
						if (MessageBox(this->window, _T("是否退出游戏"), _T("退出游戏"), MB_YESNO) == IDYES) {
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
						startFindRoad();   // 初始化寻路
						isFinding = true;  // 开始寻路
						clock_t dfs = clock();
						dfsInProgress = false;
						bfsInProgress = true;
					}
					if (msg.x >= 720 && msg.x <= 920 && msg.y >= 800 && msg.y <= 850) {
						playSound(_T("res\\music\\click.wav"), _T("click"));
						startBFS();   // 初始化寻路
						isFinding = true;  // 开始寻路
						clock_t bfs = clock();
						bfsInProgress = false;
						dfsInProgress = true;
					}
				}
			}

			if (!dfsInProgress && isFinding) {
				dfsInProgress = findRoadDFS(); // findRoadDFS返回true表示完成
				if (dfsInProgress) {
					isFinding = false;
					clock_t dfs_end = (clock() - start) / CLOCKS_PER_SEC;
					printf("DFS寻路用时 %d 秒\n", dfs_end);
				}
			}

			if (!bfsInProgress && isFinding) {
				bfsInProgress = findRoadBFS();     // stepBFS返回true表示完成
				if (bfsInProgress) {
					isFinding = false;
					clock_t bfs_end = (clock() - start) / CLOCKS_PER_SEC;
					printf("BFS寻路用时 %d 秒\n", bfs_end);
				}
			}

			// 抵达终点效果
			if (player->reachEnd()) {
				clock_t end = (clock() - start) / CLOCKS_PER_SEC;
				playSound(_T("res\\music\\tg.wav"), _T("tg"));
				printf("到终点咯,一共走了 %d 步,一共花了 %d 秒", player->Steps, end);
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
		if (MessageBox(this->window, _T("是否退出游戏"), _T("退出游戏"), MB_YESNO) == IDYES) {
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
	int v = 0;//分别用个位、十位、百位、千位表示上下左右可连通的可能
	//如果下一个位置不是边界或者不是已访问过的顶点，就表示可以连通
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
		L"确定要退出游戏吗？",
		L"提示",
		MB_YESNO | MB_ICONQUESTION);
	return ret == IDYES;   // 只有点“是”才真退出
}


void Game::DrawButton(int x, int y, int width, int height, LPCTSTR text) {
	setfillcolor(RGB(200, 200, 200));
	solidroundrect(x - width / 2, y - height / 2, x + width / 2, y + height / 2, 8, 8);

	setlinecolor(RGB(100, 100, 100));
	setlinestyle(PS_SOLID, 2);
	roundrect(x - width / 2, y - height / 2, x + width / 2, y + height / 2, 8, 8);

	settextcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	settextstyle(32, 0, _T("微软雅黑"));

	int textWidth = textwidth(text);
	int textHeight = textheight(text);
	int textX = x - textWidth / 2;
	int textY = y - textHeight / 2;
	outtextxy(textX, textY, text);
}

//音效管理
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

	// 抵达终点
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

	// DFS 下一步
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

	rode.pop(); // 回溯
	return false;
}


// BFS 寻路
// Game.cpp

// 初始化 BFS
void Game::startBFS() {
	// 清空之前的数据
	while (!bfsQueue.empty()) bfsQueue.pop();
	memset(bfsParent, 0, sizeof(bfsParent));

	int px = player->getgridX();
	int py = player->getgridY();

	bfsQueue.push(map[px][py]);
	map[px][py]->val = 2; // 标记已访问

}

// BFS 一步
bool Game::findRoadBFS() {
	if (bfsQueue.empty()) {
		bfsInProgress = false;
		return true; // 搜索结束
	}

	Map* cur = bfsQueue.front();
	bfsQueue.pop();
	int x = cur->x;
	int y = cur->y;

	// 到达终点
	if (x == END_X && y == END_Y) {
		// 回溯路径绘制
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

	// 遍历邻居
	for (auto it : cur->next) {
		if (map[it->x][it->y]->val != 2) {
			bfsQueue.push(it);
			map[it->x][it->y]->val = 2;
			bfsParent[it->x][it->y] = cur;

			// 绘制探索痕迹
			setfillcolor(BLUE);
			solidrectangle(
				it->x * (RODE + WALL) - RODE / 2,
				it->y * (RODE + WALL) - RODE / 2,
				it->x * (RODE + WALL) + RODE / 2,
				it->y * (RODE + WALL) + RODE / 2
			);
		}
	}

	return false; // BFS 还没结束
}




void Game::startFindRoad() {
	while (!rode.empty()) rode.pop(); // 清空旧栈

	int px = player->getgridX();  // 玩家当前格子
	int py = player->getgridY();

	rode.push(map[px][py]);       // 玩家位置入栈
	map[px][py]->val = 2;         // 标记走过

}