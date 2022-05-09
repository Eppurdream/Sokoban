#pragma once
#include "framework.h";
#include "resource.h"
#include "MoveInfo.h"
#include "Vector2.h"
#include <vector>
using namespace std;

#define MAX_STAGE 3
#define BW 32
#define BH 32

class Game
{
public:
    bool isEnd;

    enum class Dir
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    char arStage[MAX_STAGE][18][21] = {
    {
    "####################",
    "####################",
    "####################",
    "#####   ############",
    "#####O  ############",
    "#####  O############",
    "###  O O ###########",
    "### # ## ###########",
    "#   # ## #####  ..##",
    "# O  O   @      ..##",
    "##### ### # ##  ..##",
    "#####     ##########",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "####..  #     ######",
    "####..  # O  O  ####",
    "####..  #O####  ####",
    "####..    @ ##  ####",
    "####..  # #  O #####",
    "######### ##O O ####",
    "###### O  O O O ####",
    "######    #     ####",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "##########     @####",
    "########## O#O #####",
    "########## O  O#####",
    "###########O O #####",
    "########## O # #####",
    "##....  ## O  O  ###",
    "###...    O  O   ###",
    "##....  ############",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    };

    vector<MoveInfo> undoList; // 이건 구현하지는 않는다.
    vector<MoveInfo> redoList; // 이건 구현하지는 않는다.
    
    HWND hWndMain;
    HINSTANCE hInst;
    Game(HWND main, HINSTANCE hInst);
    ~Game();

    void DrawScreen(HDC hdc, PAINTSTRUCT& ps);
    BOOL TestEnd(); //게임이 클리어 되었는지 확인하는거
    void Move(int dir);
    void InitStage(); //스테이지 초기화함수
    void DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP hBitSheet);

    BOOL GotoNextStage();
    BOOL GoToPrevStage();
    void ErasePack(int x, int y); // 상자가 있던 자리에 맵을 복구 시켜주는 함수

    BOOL Undo(); // 수행평가
    BOOL Redo();

    int GetCurrentStage(); // 현재 스테이지 정보 알아오기

    void ToClear();
    void Ending(HDC);

private:
    char ns[18][21] = { 0, }; // 현재 플레이 중인 스테이지
    int nStage = 0;
    int nx = 0, ny = 0;
    int nMove = 0;
    Dir currentDir;
    HBITMAP hBitSheet = nullptr; // 비트맵 시트
};

