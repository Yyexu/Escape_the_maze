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
	
	Game game;		//�½���Ϸ���� Ȼ����г�ʼ������
	game.init();

	
	enum GameState state;	//����״̬�� state���л�Ϊ���˵���Ȼ����л���
	state = MAIN_MENU;
	game.render(state);

	bool running = true;	//������Ϸ������ѭ����runnnig���Կ���������Ϸ��ѭ��

	//������Ϸ��ѭ��
	while (running) {
		//���̼�� ESC�� �л�״̬Ϊ�˳� ��Ⱦ��Ӧ����
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {          
			if (game.confirmExit()) {                          
				state = EXIT;
				game.render(state);
				running = false;                               
			}                                                  
		}
		//����� Ŀǰֻ�п�ʼ��Ϸ��ť
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			//�������˵���ʱ�򣬼�¼���˵��ĵ��Ч��
			if (state == MAIN_MENU) {
				switch (msg.uMsg) {
					// �����������ʱ��
				case WM_LBUTTONDOWN:
					// ��ʼ��Ϸ��ť �����������꣬������Ϸ�����޷�����
					if (msg.x >= 500 && msg.x <= 700 && msg.y >= 425 && msg.y <= 475) {
						printf("�㰴���˿�ʼ��Ϸ\n");
						game.playSound(_T("res\\music\\click.wav"), _T("click"));  //�����������Ч
						game.stopSound(_T("bj"));
						Sleep(500);  
						state = GAME_PLAY;  //�л�״̬Ϊ��Ϸģʽ Ȼ�������Ⱦ����
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
