#pragma once
#include <string>
#include <vector>

// �� ��Ȼ�������ꡱĬ��ֵ����֤ map.h �������ά�ȱ���ͨ��
#ifndef ROW
#define ROW 30
#endif
#ifndef COL
#define COL 30
#endif

// �� ����������ʱ���������� initConfig() �ﱻ���¸�ֵ��
extern int g_ROW;
extern int g_COL;
extern int START_X;
extern int START_Y;
extern int END_X;
extern int END_Y;

// �� �����Զ���ǽͼ��1ǽ 0·��
std::vector<std::vector<bool>> loadCustomMap();

extern bool USE_CUSTOM_MAP;   // 1=���Զ���ͼ  0=����Թ�
// �� �� main() �ʼ����һ��
void initConfig();