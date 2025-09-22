#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <stack>
#include <ctime>
#include <string>
#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include "map.h"
#include "game.h"


int main()
{
	Game game;
	game.init();

	//定义状态机 state
	enum GameState state;
	state = MAIN_MENU;
	game.render(state);

	bool running = true;


	while (running) {

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {          // 1. 首次按下
			if (game.confirmExit()) {                          // 2. 点“是”才继续
				state = EXIT;
				game.render(state);
				running = false;                               // 3. 真正退出循环
			}                                                  // 4. 点“否”什么也不发生
		}

		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			printf("Mouse Clicked at: X=%d, Y=%d\n", msg.x, msg.y);
			//当是主菜单的时候，记录主菜单的点击效果
			if (state == MAIN_MENU) {
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN:
					// 开始游戏按钮
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 325 && msg.y <= 375) {
						printf("你按到了开始游戏\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
						game.stopSound(_T("bj"));
						Sleep(500);
						state = GAME_PLAY;
						game.render(state);

					}
					// 难度设置按钮
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 425 && msg.y <= 475) {
						printf("你按到了难度设置\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
					}
					// 皮肤选择按钮
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 525 && msg.y <= 575) {
						printf("你按到了皮肤选择\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
					}
					// 地图选择按钮
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 625 && msg.y <= 675) {
						printf("你按到了地图选择\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
					}
					break;
			    }
			
			}
		}
	}
	_getch();
	closegraph();
	return 0;
}
