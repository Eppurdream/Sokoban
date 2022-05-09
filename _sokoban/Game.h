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

    vector<MoveInfo> undoList; // �̰� ���������� �ʴ´�.
    vector<MoveInfo> redoList; // �̰� ���������� �ʴ´�.
    
    HWND hWndMain;
    HINSTANCE hInst;
    Game(HWND main, HINSTANCE hInst);
    ~Game();

    void DrawScreen(HDC hdc, PAINTSTRUCT& ps);
    BOOL TestEnd(); //������ Ŭ���� �Ǿ����� Ȯ���ϴ°�
    void Move(int dir);
    void InitStage(); //�������� �ʱ�ȭ�Լ�
    void DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP hBitSheet);

    BOOL GotoNextStage();
    BOOL GoToPrevStage();
    void ErasePack(int x, int y); // ���ڰ� �ִ� �ڸ��� ���� ���� �����ִ� �Լ�

    BOOL Undo(); // ������
    BOOL Redo();

    int GetCurrentStage(); // ���� �������� ���� �˾ƿ���

    void ToClear();
    void Ending(HDC);

private:
    char ns[18][21] = { 0, }; // ���� �÷��� ���� ��������
    int nStage = 0;
    int nx = 0, ny = 0;
    int nMove = 0;
    Dir currentDir;
    HBITMAP hBitSheet = nullptr; // ��Ʈ�� ��Ʈ
};

