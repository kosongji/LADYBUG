#pragma once
#include<iostream>
#include<Windows.h>

class Bug
{

public:
	Bug();
	~Bug();



public:

	float x;			// 위치
	int y;
	int y_move;			// 이동할 크기
	float x_move;
	int w;				// 이미지 사이즈
	int h;

	int impact_num = 9; // 어느색 반짝이 인가
	int impact_time = 0; // 반짝이 나타내는 시간

	int state = 0;
};


