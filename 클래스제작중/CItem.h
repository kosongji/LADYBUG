#pragma once
#include<iostream>
#include<Windows.h>

class Item
{

public:
	Item();
	~Item();



public:

	// 플레이어 위치
	float x;
	float y;
	int w;
	int h;

	// 이미지의 크기
	int picX;
	int picY;
	int picW;
	int picH;

	// 위치를 바꿔주는 변수
	int y_move;
	int x_move;

	// 생존과 죽음을 나타내는 변수
	int state = 0;

	// 누구와 충돌하였는지 알려주는 변수
	int collisionWithWho = 0;
};


