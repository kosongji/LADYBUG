#pragma comment(lib,"ws2_32");
#include <WinSock2.h>
#include <windows.h>  // 윈도우 헤더 파일 
#include <TCHAR.H>
#include <time.h>
#include <math.h>
#include <iostream>
#include <atlimage.h>
//sound h
#include <conio.h>


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512
#define CONSOLE 1
 
using namespace std;

//디바이스 컨테스트 얻기
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";

HPEN hPen, oldPen;
HBRUSH hBrush, oldBrush;
HFONT myFont, oldFont;

SOCKET server_sock;
HANDLE hReadEvent, hWriteEvent;

static TCHAR str[100];
int accessNumber = 0;


// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;
	int received;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(server_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	
	while (1)
	{
		received = recv(server_sock, (char*)&accessNumber, sizeof(int), 0);
	}
	printf("TEST");
}

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

	//이벤트 생성
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL) return 1;
	hWriteEvent = CreateEvent(NULL, false, false, NULL);
	if (hWriteEvent == NULL) return 1;
	
	//소켓 통신 스레드 생성
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	//이벤트 제거
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	closesocket(server_sock);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
HINSTANCE hInst;



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, memdc;               //디바이스 콘텍스트를 저장할 변수 HDC와 
	PAINTSTRUCT ps;            //출력영역(디콘)에 대한 정보를 저장할 변수 PS선언.
	static HBITMAP hBit, oldBit;

	// 데이터 통신에 사용할 변수
	//SOCKET server_sock;

	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		SetTimer(hWnd, 1, 17, NULL);
	
		break;
	case WM_CHAR:
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_TIMER:

		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hDC);

		hBit = CreateCompatibleBitmap(hDC, 1000, 800);
		SelectObject(memdc, hBit);

	
		switch (accessNumber)
		{
		SetTextColor(memdc, RGB(255, 228, 0));
		SetBkMode(memdc, TRANSPARENT);
		myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("고딕"));
		oldFont = (HFONT)SelectObject(memdc, myFont);
		case 0: 

			break;

		case 1:

			wsprintf(str, L"1접속되었습니다");
			TextOut(memdc, 10, 10, str, lstrlen(str));

			SelectObject(memdc, oldFont);
			DeleteObject(myFont);
			break;

		case 2:
			wsprintf(str, L"1접속되었습니다");
			TextOut(memdc, 10, 10, str, lstrlen(str));

			SelectObject(memdc, oldFont);
			DeleteObject(myFont);

			wsprintf(str, L"2접속되었습니다");
			TextOut(memdc, 10, 50, str, lstrlen(str));

			SelectObject(memdc, oldFont);
			DeleteObject(myFont);
			break;

		}

		BitBlt(hDC, 0, 0, 500, 800, memdc, 0, 0, SRCCOPY);

		DeleteObject(hBit);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}