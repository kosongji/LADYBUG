#pragma once
#include<iostream>
#include<Windows.h>

class Item
{

public:
	Item();
	~Item();



public:

	// �÷��̾� ��ġ
	float x;
	float y;
	int w;
	int h;

	// �̹����� ũ��
	int picX;
	int picY;
	int picW;
	int picH;

	// ��ġ�� �ٲ��ִ� ����
	int y_move;
	int x_move;

	// ������ ������ ��Ÿ���� ����
	int state = 0;

	// ������ �浹�Ͽ����� �˷��ִ� ����
	int collisionWithWho = 0;
};


