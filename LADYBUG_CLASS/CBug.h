#pragma once
#include<iostream>
#include<Windows.h>

class Bug
{

public:
	Bug();
	~Bug();



public:

	float x;			// ��ġ
	int y;
	int y_move;			// �̵��� ũ��
	float x_move;
	int w;				// �̹��� ������
	int h;

	int impact_num = 9; // ����� ��¦�� �ΰ�
	int impact_time = 0; // ��¦�� ��Ÿ���� �ð�

	int state = 0;
};


