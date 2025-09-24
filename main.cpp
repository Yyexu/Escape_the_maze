#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <stack>
#include <ctime>
#include <string>
#include <windows.h>
#pragma comment(lib, "winmm.lib")

#include "map.h"
#include "game.h"
#include "config.h"


int main()
{	
	
	Game game;		//新建游戏对象 然后进行初始化操作
	game.init();

	
	enum GameState state;	//定义状态机 state，切换为主菜单，然后进行绘制
	state = MAIN_MENU;
	game.render(state);

	bool running = true;	//控制游戏整个大循环，runnnig可以控制整个游戏的循环

	//整个游戏大循环
	while (running) {
		//键盘检测 ESC键 切换状态为退出 渲染对应界面
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {          
			if (game.confirmExit()) {                          
				state = EXIT;
				game.render(state);
				running = false;                               
			}                                                  
		}
		//鼠标检测 目前只有开始游戏按钮
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			//当是主菜单的时候，记录主菜单的点击效果
			if (state == MAIN_MENU) {
				switch (msg.uMsg) {
					// 当按下左键的时候
				case WM_LBUTTONDOWN:
					// 开始游戏按钮 检测的是死坐标，这样游戏界面无法伸缩
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 425 && msg.y <= 475) {
						printf("你按到了开始游戏\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));  //点击后的鼠标声效
						game.stopSound(_T("bj"));
						Sleep(500);  
						state = GAME_PLAY;  //切换状态为游戏模式 然后进行渲染绘制
						game.render(state);

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
