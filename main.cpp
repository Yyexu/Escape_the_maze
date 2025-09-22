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

	//����״̬�� state
	enum GameState state;
	state = MAIN_MENU;
	game.render(state);

	bool running = true;


	while (running) {

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {          // 1. �״ΰ���
			if (game.confirmExit()) {                          // 2. �㡰�ǡ��ż���
				state = EXIT;
				game.render(state);
				running = false;                               // 3. �����˳�ѭ��
			}                                                  // 4. �㡰��ʲôҲ������
		}

		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			printf("Mouse Clicked at: X=%d, Y=%d\n", msg.x, msg.y);
			//�������˵���ʱ�򣬼�¼���˵��ĵ��Ч��
			if (state == MAIN_MENU) {
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN:
					// ��ʼ��Ϸ��ť
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 325 && msg.y <= 375) {
						printf("�㰴���˿�ʼ��Ϸ\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
						game.stopSound(_T("bj"));
						Sleep(500);
						state = GAME_PLAY;
						game.render(state);

					}
					// �Ѷ����ð�ť
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 425 && msg.y <= 475) {
						printf("�㰴�����Ѷ�����\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
					}
					// Ƥ��ѡ��ť
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 525 && msg.y <= 575) {
						printf("�㰴����Ƥ��ѡ��\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));
					}
					// ��ͼѡ��ť
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 625 && msg.y <= 675) {
						printf("�㰴���˵�ͼѡ��\n");
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
