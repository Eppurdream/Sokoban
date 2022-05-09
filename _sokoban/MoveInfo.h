#pragma once


class MoveInfo
{
public:
	char dx : 3;
	char dy : 3;
	char bWidthPack : 2;
	int dir;

	MoveInfo(int dx, int dy, bool bWidthPack, int dir)
	{
		this->dx = dx;
		this->dy = dy;
		this->bWidthPack = bWidthPack;
		this->dir = dir;
		// 비트 필드는 원하는 비트만 잘라서 가져가기 때문에 하단 비트만 잘려나간다.
	}
};

