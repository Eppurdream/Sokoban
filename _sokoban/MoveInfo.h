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
		// ��Ʈ �ʵ�� ���ϴ� ��Ʈ�� �߶� �������� ������ �ϴ� ��Ʈ�� �߷�������.
	}
};

