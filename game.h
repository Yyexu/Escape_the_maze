#ifndef GAME_H
#define GAME_H

#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include "Player.h"

enum GameState
{
    MAIN_MENU = 0,
    GAME_PLAY = 1,
    DIFFICULTY_SETTING = 2,
    SKIN_SELECTION = 3,
    MAP_SELECTION = 4,
    EXIT = -1
};

class Game {
public:
    LPCTSTR Game_Name;
    int GAME_WIDTH;
    int GAME_HEIGHT;
    COLORREF Bk_color;
    HWND window;
    HWND hConsole{};
    Player* player{};

    int visit(int x, int y);
    void init();
    void render(enum GameState state);
    void DrawButton(int x, int y, int width, int height, LPCTSTR text);
    void playSound(LPCTSTR file, LPCTSTR alias);
    void stopSound(LPCTSTR alias);
    static bool confirmExit();
};


#endif