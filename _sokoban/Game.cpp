#include "Game.h"

Game::Game(HWND main, HINSTANCE hInst)
{
	hWndMain = main;
	hBitSheet = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHEET));
    this->hInst = hInst;
	nStage = 0;
    isEnd = false;
}

Game::~Game()
{
	DeleteObject(hBitSheet);
}

void Game::DrawScreen(HDC hdc, PAINTSTRUCT& ps)
{
    int iBit = 3;
    TCHAR userMsg[256];
    //EMPTY, MAN, PACK, TARGET, WALL
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++) {
            switch (ns[y][x])
            {
            case '#':
                iBit = 0;
                break;
            case '.':
                iBit = 2;
                break;
            case 'O':
                iBit = 1;
                break;
            case ' ':
                iBit = 7;
                break;
            }

            DrawBitmap(hdc, iBit, x * BW, y * BH, hBitSheet);
        }
    }
    
    DrawBitmap(hdc, 3 + (int)currentDir, nx * BW, ny * BH, hBitSheet);

    wsprintf(userMsg, _T("SOKOBAN !"));
    TextOut(hdc, 700, 20, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("Q : 종료, R : 다시시작"));
    TextOut(hdc, 700, 40, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("N : 다음, P : 이전"));
    TextOut(hdc, 700, 60, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("Z : Undo, X : REDO"));
    TextOut(hdc, 700, 80, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("스테이지 :  %d"), nStage);
    TextOut(hdc, 700, 100, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("이동횟수 :  %d"), nMove);
    TextOut(hdc, 700, 120, userMsg, lstrlen(userMsg));
}

BOOL Game::TestEnd()
{
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (arStage[nStage][y][x] == '.') {
                if (ns[y][x] != 'O')
                {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

void Game::Move(int dir)
{
    int dx = 0, dy = 0; // 델타x, 델타y
    BOOL bWithPack = FALSE;

    switch (dir)
    {
    case VK_LEFT:
        dx = -1;
        currentDir = Dir::LEFT;
        break;
    case VK_RIGHT:
        dx = 1;
        currentDir = Dir::RIGHT;
        break;
    case VK_UP:
        dy = -1;
        currentDir = Dir::UP;
        break;
    case VK_DOWN:
        dy = 1;
        currentDir = Dir::DOWN;
        break;
    default:
        break;
    }

    if (ns[ny + dy][nx + dx] != '#')
    {
        if (ns[ny + dy][nx + dx] == 'O')
        {
            if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.')
            {
                ErasePack(nx + dx, ny + dy);

                ns[ny + dy * 2][nx + dx * 2] = 'O';
                bWithPack = TRUE;
            }
            else
            {
                return;
            }
        }

        nx += dx;
        ny += dy;
        nMove++;

        MoveInfo mInfo(dx, dy, bWithPack, (int)currentDir);
        undoList.push_back(mInfo);
        redoList.clear();

        InvalidateRect(hWndMain, NULL, FALSE);
    }
}

void Game::InitStage()
{
    memcpy(ns, arStage[nStage], sizeof(ns)); //메모리 카피

    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == '@') {  //플레이어의 시작위치
                nx = x;
                ny = y;
                ns[y][x] = ' ';  //해당위치에 플레이어가 있으니 공백처리
            }
        }
    }

    nMove = 0;
    undoList.clear();
    InvalidateRect(hWndMain, NULL, TRUE);
}

void Game::DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP hBitSheet)
{
    HDC hMemDC;
    HBITMAP hOldBitMap;
    BITMAP bit;

    hMemDC = CreateCompatibleDC(hdc); //메모리 DC생성
    hOldBitMap = static_cast<HBITMAP>(SelectObject(hMemDC, hBitSheet));

    GetObject(hBitSheet, sizeof(BITMAP), &bit);

    StretchBlt(hdc, x, y, 32, 32, hMemDC, idx * 32, 0, 32, 32, SRCCOPY);

    SelectObject(hMemDC, hOldBitMap);
    DeleteDC(hMemDC);
}

int Game::GetCurrentStage()
{
    return nStage;
}

void Game::ToClear()
{
    vector<Vector2> boxList;
    vector<Vector2> pointList;

    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (ns[y][x] == 'O') {  //플레이어의 시작위치
                Vector2 vec(x, y);

                boxList.push_back(vec);
            }
            if (ns[y][x] == '.')
            {
                Vector2 vec(x, y);

                pointList.push_back(vec);
            }
        }
    }

    for (int i = 0; i < boxList.size(); i++)
    {
        ns[boxList[i].y][boxList[i].x] = ' ';
        ns[pointList[i].y][pointList[i].x] = 'O';
    }

    TCHAR userMsg[256];

    InvalidateRect(hWndMain, NULL, TRUE);

    bool check = false;
    if (nStage < MAX_STAGE - 1)
    {
        wsprintf(userMsg, _T("스테이지 클리어"));

        nStage++;
    }
    else
    {
        check = true;
        wsprintf(userMsg, _T("모든 스테이지를 클리어 했습니다"));
    }
    MessageBox(hWndMain, userMsg, _T("알림"), MB_OK);

    if (check)
    {
        isEnd = true;
        InvalidateRect(hWndMain, NULL, TRUE);
    }

    if (nStage < MAX_STAGE && !isEnd)
    {
        InitStage(); // 스테이지 초기화
    }
}

void Game::Ending(HDC hdc)
{
    HDC hMemDC;
    HBITMAP hEndBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LAND));
    HBITMAP hOldBitMap;
    BITMAP bit;

    hMemDC = CreateCompatibleDC(hdc); //메모리 DC생성
    hOldBitMap = static_cast<HBITMAP>(SelectObject(hMemDC, hEndBitMap));

    GetObject(hEndBitMap, sizeof(BITMAP), &bit);

    BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, hMemDC, 0, 0, SRCCOPY);

    SelectObject(hMemDC, hOldBitMap);
    DeleteDC(hMemDC);
}

BOOL Game::GotoNextStage()
{
    if (nStage < MAX_STAGE - 1)
    {
        nStage++;
        InitStage(); // 스테이지 초기화
        return TRUE;
    }

    return FALSE;
}

BOOL Game::GoToPrevStage()
{
    if (nStage > 0)
    {
        nStage--;
        InitStage();
        return TRUE;
    }
    return FALSE;
}

void Game::ErasePack(int x, int y)
{
    ns[y][x] = arStage[nStage][y][x] == '.' ? '.' : ' ';
}

BOOL Game::Undo()
{
    if (undoList.size() == 0) return FALSE;

	MoveInfo info =  undoList[undoList.size() - 1];

	if (info.bWidthPack)
	{
        ErasePack(nx + info.dx, ny + info.dy);

		ns[ny][nx] = 'O';
	}

	nx -= info.dx;
	ny -= info.dy;

    redoList.push_back(info);
	undoList.pop_back();

    nMove--;
    currentDir = (Dir)info.dir;
    InvalidateRect(hWndMain, NULL, TRUE);
    return TRUE;
}

BOOL Game::Redo()
{
    if (redoList.size() == 0) return FALSE;

    MoveInfo info = redoList[redoList.size() - 1];

    if (info.bWidthPack)
    {
        ErasePack(nx + info.dx, ny + info.dy);

        ns[ny + info.dy * 2][nx + info.dx * 2] = 'O';
    }

    nx += info.dx;
    ny += info.dy;

    undoList.push_back(info);
    redoList.pop_back();

    nMove++;
    currentDir = (Dir)info.dir;
    InvalidateRect(hWndMain, NULL, TRUE);
    return TRUE;
}

// Undo, Redo