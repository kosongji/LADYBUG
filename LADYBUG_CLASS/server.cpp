#include <windows.h>  // 윈도우 헤더 파일 
#include <TCHAR.H>
#include <time.h>
#include <math.h>
#include <iostream>
#include <atlimage.h>

//sound h
#include <conio.h>
#include <fmod.h>

//class
#include "CPlayer.h"
#include "CBug.h"
#include "CItem.h"

#define ITEM_TYPE 8
FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound[14];
FMOD_CHANNEL *g_Channel[14];

/// <summary>
/// FMOD System INIT fucntion
/// </summary>

// Sound
/*
void Init()
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 14, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateSound(g_System, "배경음2.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	FMOD_System_CreateSound(g_System, "1.wav", FMOD_DEFAULT, 0, &g_Sound[1]);
	FMOD_System_CreateSound(g_System, "방해2.wav", FMOD_DEFAULT, 0, &g_Sound[2]);
	FMOD_System_CreateSound(g_System, "3.wav", FMOD_DEFAULT, 0, &g_Sound[3]);
	FMOD_System_CreateSound(g_System, "4.wav", FMOD_DEFAULT, 0, &g_Sound[4]);
	FMOD_System_CreateSound(g_System, "방해1.wav", FMOD_DEFAULT, 0, &g_Sound[5]);
	FMOD_System_CreateSound(g_System, "6.wav", FMOD_DEFAULT, 0, &g_Sound[6]);
	FMOD_System_CreateSound(g_System, "7.wav", FMOD_DEFAULT, 0, &g_Sound[7]);
	FMOD_System_CreateSound(g_System, "8.wav", FMOD_DEFAULT, 0, &g_Sound[8]);
	FMOD_System_CreateSound(g_System, "9.wav", FMOD_DEFAULT, 0, &g_Sound[9]);
	FMOD_System_CreateSound(g_System, "10.wav", FMOD_DEFAULT, 0, &g_Sound[10]);
	FMOD_System_CreateSound(g_System, "배경음1.mp3", FMOD_DEFAULT, 0, &g_Sound[11]);
	FMOD_System_CreateSound(g_System, "10gas.wav", FMOD_DEFAULT, 0, &g_Sound[12]);

	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[0], 0, &g_Channel[0]);
}

/// FMOD System Release fucntion
void Release()
{
	FMOD_Sound_Release(g_Sound[1]);
	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}
*/

using namespace std;

Player::Player()
{

}

Player::~Player()
{
}

Bug::Bug()
{

}

Bug::~Bug()
{
}

Item::Item()
{

}

Item::~Item()
{
}

//디바이스 컨테스트 얻기
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);


	hWnd = CreateWindow(lpszClass, L"레이디 버그", WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_THICKFRAME, 0, 0, 500, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}