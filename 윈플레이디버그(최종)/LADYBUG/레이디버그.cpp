#include <windows.h>  // ������ ��� ���� 
#include <TCHAR.H>
#include <time.h>
#include <math.h>
#include <iostream>
#include <atlimage.h>

//sound h
#include <conio.h>
#include <fmod.h>

#define ITEM_TYPE 8
FMOD_SYSTEM *g_System;
FMOD_SOUND *g_Sound[14];
FMOD_CHANNEL *g_Channel[14];

/// <summary>
/// FMOD System INIT fucntion
/// </summary>

void Init()
{
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 14, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateSound(g_System, "�����2.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	FMOD_System_CreateSound(g_System, "1.wav", FMOD_DEFAULT, 0, &g_Sound[1]);
	FMOD_System_CreateSound(g_System, "����2.wav", FMOD_DEFAULT, 0, &g_Sound[2]);
	FMOD_System_CreateSound(g_System, "3.wav", FMOD_DEFAULT, 0, &g_Sound[3]);
	FMOD_System_CreateSound(g_System, "4.wav", FMOD_DEFAULT, 0, &g_Sound[4]);
	FMOD_System_CreateSound(g_System, "����1.wav", FMOD_DEFAULT, 0, &g_Sound[5]);
	FMOD_System_CreateSound(g_System, "6.wav", FMOD_DEFAULT, 0, &g_Sound[6]);
	FMOD_System_CreateSound(g_System, "7.wav", FMOD_DEFAULT, 0, &g_Sound[7]);
	FMOD_System_CreateSound(g_System, "8.wav", FMOD_DEFAULT, 0, &g_Sound[8]);
	FMOD_System_CreateSound(g_System, "9.wav", FMOD_DEFAULT, 0, &g_Sound[9]);
	FMOD_System_CreateSound(g_System, "10.wav", FMOD_DEFAULT, 0, &g_Sound[10]);
	FMOD_System_CreateSound(g_System, "�����1.mp3", FMOD_DEFAULT, 0, &g_Sound[11]);
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

#define CONSOLE 1
#define MONSTER 500
#define DEBUG 1

using namespace std;

//����̽� ���׽�Ʈ ���
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

	//if (CONSOLE)
	//{
	//	AllocConsole();
	//	freopen("CONOUT$", "wt", stdout);
	//}

	hWnd = CreateWindow(lpszClass, L"���̵� ����", WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_THICKFRAME, 0, 0, 500, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


typedef struct Move {
	//int state;

	float x;
	float y;
	int w;
	int h;

	int picX;
	int picY;
	int picW;
	int picH;

	int y_move;
	int x_move;

	int count = 0;
	int state = 0;

	int collisionWithWho = 0;
};
HINSTANCE hInst;

static struct BUG
{
	float x;
	int y;
	int y_move;
	float x_move;
	int w;
	int h;

	int impact_num = 9;
	int impact_time = 0;

	int state = 0;

}bug[MONSTER];

float bug_x_move(BUG bug, Move player)
{
	float bug_lean = ((player.y - bug.y) / (player.x - bug.x));
	bug.x_move = bug.y_move / bug_lean;
	return bug.x_move;
}

//////////////////////////////////////////////////////////////////////////
//static int player_Flag = 0;
//////////////////////////////////////////////////////////////////////////

static int item_1_Flag[10] = { 0, };   //�������� ������ �÷��׸� 1�� �ø���.
static int item_1_frame[10] = { 0, };//���� ������ ����
static int item_1_carry[10] = { 0, };//������ ������ ������
									 //�� ���ư��°�
void ITEM1(Move &item_1, int i);

static int item_8_Flag[10] = { 0, };
static int item_8_frame[10] = { 0, };
//�ڽ��� ������
void ITEM8(Move &player, Move &item_8, int i);

static int item_6_Flag[10] = { 0, };
static int item_6_frame[10] = { 0, };
static int item_6_direction[10] = { 0, };
static int item_6_bounce[10] = { 0, };
//����� ������
void ITEM6(Move &item_6, int i);
//����� ����
void ITEM6_DIRECTION(Move &item_6, int i);
//����� Ƣ���
void ITEM6_MOVE(Move &item_6, int i);

static int item_10_Flag[10] = { 0, };
//�нż�
void ITEM10(Move &player, Move &item_10, int i);


//���ؿ�� 2 �÷���
static int interrupt_ITEM2_Flag = 0;
static int interrupt_ITEM2_timer = 0;
static float interrupt_ITEM2_frame = 0;

//���� Ű
static int key = 0;

void playerCollisionCheck(Move &player)
{
	if (key != 1)
	{
		
		//�÷��̾� + ���� �浹
			for (int i = 0; i < MONSTER; ++i)
			{
				if (bug[i].state != 0)
				{
					//�÷��̾� top���� �浹
					if ((bug[i].x <= player.x && player.x <= bug[i].x + 30) &&
						(0 <= bug[i].y + 30 - player.y && bug[i].y + 30 - player.y <= 20))
					{
						player.state = 0;
					}
					else if ((bug[i].x <= player.x + 30 && player.x + 30 <= bug[i].x + 30) &&
						(0 <= player.y - bug[i].y + 30 && player.y - bug[i].y + 30 <= 20))
					{
						player.state = 0;
					}

					//�÷��̾� left���� �浹
					else if ((bug[i].y <= player.y && player.y <= bug[i].y + 30) &&
						(0 <= player.x - bug[i].x + 30 && player.x - bug[i].x + 30 <= 20))
					{
						player.state = 0;
					}
					else if ((bug[i].y <= player.y + 30 && player.y + 30 <= bug[i].y + 30) &&
						(0 <= player.x - bug[i].x + 30 && player.x - bug[i].x + 30 <= 20))
					{
						player.state = 0;
					}

					//�÷��̾� right���� �浹
					else if ((bug[i].y <= player.y  && player.y <= bug[i].y + 30) &&
						(0 <= bug[i].x - player.x + 30 && bug[i].x - player.x + 30 <= 20))
					{
						player.state = 0;
					}
					else if ((bug[i].y <= player.y + 30 && player.y + 30 <= bug[i].y + 30) &&
						(0 <= bug[i].x - player.x + 30 && bug[i].x - player.x + 30 <= 20))
					{
						player.state = 0;
					}

					//�÷��̾� bottom���� �浹
					else if ((bug[i].x <= player.x && player.x <= bug[i].x + 30) &&
						(0 <= bug[i].y - player.y + 30 && bug[i].y - player.y + 30 <= 20))
					{
						player.state = 0;
					}
					else if ((bug[i].x <= player.x + 30 && player.x + 30 <= bug[i].x + 30) &&
						(0 <= bug[i].y - player.y + 30 && bug[i].y - player.y + 30 <= 20))
					{
						player.state = 0;
					}
				}
			}
		
	}
}

//������ + ���� �浹
void collisionCheck(Move &item)
{

	for (int i = 0; i < MONSTER; ++i)
	{
		if (bug[i].state != 0)
		{
			//������ top���� �浹
			if ((item.x <= bug[i].x + 15 && bug[i].x + 15 <= item.x + item.w) && (0 <= item.y - (bug[i].y + 15) && item.y - (bug[i].y + 15) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}
			else if ((item.x <= bug[i].x && bug[i].x <= item.x + item.w) && (0 <= item.y - (bug[i].y + 15) && item.y - (bug[i].y + 15) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}

			//������ left���� �浹
			else if ((item.y <= bug[i].y && bug[i].y <= item.y + item.h) && (0 <= item.x - (bug[i].x + 15) && item.x - (bug[i].x + 15) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}
			else if ((item.y <= bug[i].y + 15 && bug[i].y + 15 <= item.y + item.h) && (0 <= item.x - (bug[i].x + 15) && item.x - (bug[i].x + 15) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}

			//������ right���� �浹
			else if ((item.y <= bug[i].y && bug[i].y <= item.y + item.h) && (0 <= bug[i].x - (item.x + item.w) && bug[i].x - (item.x + item.w) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}
			else if ((item.y <= bug[i].y + 15 && bug[i].y + 15 <= item.y + item.h) && (0 <= bug[i].x - (item.x + item.w) && bug[i].x - (item.x + item.w) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}

			//������ bottom���� �浹
			else if ((item.x <= bug[i].x + 15 && bug[i].x + 15 <= item.x + item.w) && (0 <= bug[i].y - (item.y + item.h) && bug[i].y - (item.y + item.h) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}
			else if ((item.x <= bug[i].x && bug[i].x <= item.x + item.w) && (0 <= bug[i].y - (item.y + item.h) && bug[i].y - (item.y + item.h) <= 20))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}

			//������ ���ο��� �浹
			else if ((item.x <= bug[i].x + 15 && bug[i].x + 15 <= item.x + item.w) && (item.x <= bug[i].x && bug[i].x <= item.x + item.w) &&
				(item.y <= bug[i].y && bug[i].y <= item.y + item.h) && (item.y <= bug[i].y + 15 && bug[i].y + 15 <= item.y + item.h))
			{
				bug[i].impact_num = (rand() % 8);
				bug[i].state = 0;
			}
		}

	}
}

//�����۵�� + �÷��̾� �浹
int IsItemCollisionCheck(Move &item, Move &player)
{
	//for (int i = 0; i < MONSTER; ++i)
	{
		//������ top���� �浹
		if (((item.x <= player.x + 30 && player.x + 30 <= item.x + item.w) && (0 <= item.y - (player.y + 30) && item.y - (player.y + 30) <= 10)) ||
			((item.x <= player.x && player.x <= item.x + item.w) && (0 <= item.y - (player.y + 30) && item.y - (player.y + 30) <= 10)))
		{
			return 1;
		}

		//������ left���� �浹
		else if (((item.y <= player.y && player.y <= item.y + item.h) && (0 <= item.x - (player.x + 30) && item.x - (player.x + 30) <= 10)) ||
			((item.y <= player.y + 30 && player.y + 30 <= item.y + item.h) && (0 <= item.x - (player.x + 30) && item.x - (player.x + 30) <= 10)))
		{
			return 1;

		}

		//������ right���� �浹
		else if (((item.y <= player.y && player.y <= item.y + item.h) && (0 <= player.x - (item.x + item.w) && player.x - (item.x + item.w) <= 10)) ||
			((item.y <= player.y + 30 && player.y + 30 <= item.y + item.h) && (0 <= player.x - (item.x + item.w) && player.x - (item.x + item.w) <= 10)))
		{
			return 1;

		}

		//������ bottom���� �浹
		else if (((item.x <= player.x + 30 && player.x + 30 <= item.x + item.w) && (0 <= player.y - (item.y + item.h) && player.y - (item.y + item.h) <= 10)) ||
			((item.x <= player.x && player.x <= item.x + item.w) && (0 <= player.y - (item.y + item.h) && player.y - (item.y + item.h) <= 10)))
		{
			return 1;

		}

		//������ ���ο��� �浹
		else if ((item.x <= player.x + 30 && player.x + 30 <= item.x + item.w) && (item.x <= player.x && player.x <= item.x + item.w) &&
			(item.y <= player.y && player.y <= item.y + item.h) && (item.y <= player.y + 30 && player.y + 30 <= item.y + item.h))
		{
			return 1;

		}

	}
	return 0;
}

//		Rectangle(memdc, 350, 500, 500, 550);
//Rectangle(memdc, 350, 570, 500, 620);
//Rectangle(memdc, 350, 640, 500, 690);

//�����
int check_menu(int mx, int my)
{
	if (350 < mx&&mx < 500)
	{
		if (500 < my&&my < 550)
		{
			return 1;
		}
		else if (570 < my&&my < 620)
		{
			return 2;
		}
		else if (640 < my&&my < 690)
		{
			return 3;
		}

	}
	return 0;
}



//int check_collision(BUG* bug, Move* item)// �浹 üũ
//{
//
//	if (bug->state != 0)
//	{
//		if (item->x < bug->x&&bug->x < item->x + item->w || item->x < bug->x + bug->w&& bug->x + bug->w < item->x + item->w)
//		{
//			if (item->y < bug->y&&bug->y <item->y + item->h || item->y < bug->y + bug->h&& bug->y + bug->h < item->y + item->h)
//			{
//				bug->impact_num = (rand() % 8);
//				return  0;
//			}
//		}
//	}
//	return bug->state;
//}
//
//void item_3_BARRIER(BUG *bug, Move *Barrier, BOOL *item_3, Move *player, int *Br_time)
//{
//	Barrier->x = player->x - 45;
//	Barrier->y = player->y - 45;
//	(*Br_time)++;
//	if ((*Br_time) == 40)
//	{
//		(*Br_time) = 0;
//		(*item_3) = false;
//	}
//	for (int i = 0; i < 500; i++)
//	{
//		bug[i].state = check_collision(&bug[i], Barrier);
//	}
//}
//
//void item_4_BEE(BUG *bug, Move *Bee, BOOL *item_4)
//{
//	Bee->y -= 7;
//	if (Bee->y < -200)
//	{
//		(*item_4) = false;
//	}
//	Bee->count++;
//	for (int i = 0; i < 500; i++)
//	{
//		bug[i].state = check_collision(&bug[i], Bee);
//	}
//}
//
//void item_7_Baby_bug(BUG *bug, Move *Baby_bug, BOOL *item_7, Move *player, int* bbug_num)
//{
//
//
//	if ((*bbug_num) < 24)
//	{
//		Baby_bug[(*bbug_num)].state = 1;
//		(*bbug_num)++;
//
//	}
//	for (int i = 0; i < 24; i++)
//	{
//		if (Baby_bug[i].state == 1)
//		{
//			Baby_bug[i].x += Baby_bug[i].x_move;
//			Baby_bug[i].y += Baby_bug[i].y_move;
//		}
//		else if (Baby_bug[i].state == 0)
//		{
//			Baby_bug[i].x = player->x;
//			Baby_bug[i].y = player->y;
//		}
//	}
//
//
//	for (int i = 0; i < 500; i++)
//	{
//		for (int j = 0; j < 24; j++)
//		{
//			bug[i].state = check_collision(&bug[i], &Baby_bug[j]);
//		}
//
//	}
//
//	if (Baby_bug[23].x < -200)
//	{
//		for (int i = 0; i < 24; i++)
//		{
//			Baby_bug[i].state = 0;
//		}
//
//		(*item_7) = false;
//		*bbug_num = 0;
//	}
//
//}
//
//void item_9_CMLINE(BUG *bug, Move *CM_line, BOOL *item_9)
//{
//	CM_line->y -= 10;
//	if (CM_line->y < -200)
//	{
//		(*item_9) = false;
//	}
//	CM_line->count++;
//	for (int i = 0; i < 500; i++)
//	{
//		bug[i].state = check_collision(&bug[i], CM_line);
//	}
//
//}
//void special_bug(BUG* bug ,BOOL* sp_bog_on, int* count)
//{
//	(*count)++;
//	if ((*count) > 100)
//	{
//		(*count) = 0;
//		(*sp_bog_on) = false;
//	}
//	if ((rand() % 3) == 0)
//	{
//		bug->state = 2;
//		bug->w = 60;
//		bug->h = 60;
//	}
//}





LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, memdc;               //����̽� ���ؽ�Ʈ�� ������ ���� HDC�� 
	PAINTSTRUCT ps;            //��¿���(����)�� ���� ������ ������ ���� PS����.
	static HBITMAP hBit, oldBit;

	static FMOD_BOOL IsPlaying;
	static float volume = 0.5f;
	//
	static CImage BACKGROUND;
	static Move backGround1;
	static Move backGround2;
	//
	static CImage ITEM_1;
	static Move item_1[10];
	static int item_1_count;    //�迭���²�� ����
								//
	static CImage ITEM_6;
	static Move item_6[10];
	static int item_6_count;   //�迭���²�� ����

							   //
	static CImage ITEM_8;
	static Move item_8[10];
	static int item_8_count;   //�迭���²�� ����
							   //
	static CImage ITEM_10;
	static Move item_10[10];
	static int item_10_count;   //�迭���²�� ����
								//
	static CImage PLAYER;
	static Move player;
	static CImage SECONDPLAYER;
	static Move SecondPlayer;
	//
	static CImage BUG;
	//
	//static CImage INTERRUPT_ITEM1;
	static CImage INTERRUPT_ITEM2;

	static TCHAR str[100];


	//
	static CImage image1;
	static CImage image2;
	static CImage image3;
	static CImage image4;
	static CImage image5;
	static CImage image6;
	static CImage image7;
	static CImage image8;
	static CImage image9;
	static CImage image10;

	//
	static int item_Drop_Timer;
	static Move item_Drop[20];
	static int item_Count;
	//

	//static CImage S_BUG;
	//static CImage BEE;
	//static CImage COSMOS;
	//static CImage BABYBUG;
	//static CImage BARRIER;

	

	static CImage impact[8];

/*	static BOOL sp_bug=false;
	static BOOL item_3[10];
	static BOOL item_4[10];
	static BOOL item_7[10];
	static BOOL item_9[10];  */  
	//
	static CImage GAME_OVER[2];
	static CImage menu[4];
	static CImage item_menu[6];
	//
	//static Move Barrier[10];//���� ��ȣ��
	//static Move Bee[10];// ��
	//static Move Baby_bug[10][24];// ���̺� ����
	//static Move CM_line[10];// ���� �ڽ���


	static BOOL Gameover = false;
	//static int Br_time[10];
	//static int bbug_num[10];
	static int bug_num;
	//static int sp_bug_time=0;
	static float mx, my;
	static int score;
	
	//static int Br_num = 0;
	//static int Bee_num = 0;
	//static int Bb_num = 0;
	//static int Cm_num = 0;

	static float distance = 0;
	static float rotation = 0;
	static float speed = 10;

	static BOOL pause = false;
	static BOOL START = false;
	static BOOL mode_2p = false;

	static int menu_check = 0;
	static int over_check = 0;
	static int itme_menu_check = 6;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HFONT myFont, oldFont;


	switch (iMessage)
	{
	case WM_CREATE:

		Init();
		FMOD_Channel_IsPlaying(g_Channel[0], &IsPlaying);
		/*	if (volume < 1.0f)
		volume += 50.0f;
		FMOD_Channel_SetVolume(g_Channel[5], volume);*/


		srand((unsigned)time(NULL));
		SetTimer(hWnd, 1, 17, NULL);

		item_Drop_Timer = 0;
		item_Count = 0;
		for (int i = 0; i < 20; ++i)
		{
			item_Drop[i].state = 0;
		}

		item_1_count = 0;
		item_6_count = 0;
		item_8_count = 0;
		item_10_count = 0;

		for (int i = 0; i < 10; ++i)
			item_6_direction[i] = 0;

		BACKGROUND.Load(TEXT("��¥���1.png"));
		ITEM_1.Load(TEXT("������11.png"));
		//S_BUG.Load(TEXT("Ư�� ��.bmp"));
		//BEE.Load(TEXT("��.png"));
		//BABYBUG.Load(TEXT("���̺� ����.bmp"));
		//COSMOS.Load(TEXT("���� �ڽ���.bmp"));
		//BARRIER.Load(TEXT("���� ��ȣ��3.png"));
		ITEM_6.Load(TEXT("������6.png"));
		ITEM_8.Load(TEXT("������8.png"));
		ITEM_10.Load(TEXT("������10.png"));
		PLAYER.Load(TEXT("���̵����.bmp"));
		SECONDPLAYER.Load(TEXT("���̵����2.png"));
		BUG.Load(TEXT("��.bmp"));
		//INTERRUPT_ITEM1.Load(TEXT("����1.png"));
		INTERRUPT_ITEM2.Load(TEXT("����2.png"));

		image1.Load(TEXT("1.png"));
		image2.Load(TEXT("2.png"));
		image3.Load(TEXT("3.png"));
		image4.Load(TEXT("4.png"));
		image5.Load(TEXT("5.png"));
		image6.Load(TEXT("6.png"));
		image7.Load(TEXT("7.png"));
		image8.Load(TEXT("8.png"));
		image9.Load(TEXT("9.png"));
		image10.Load(TEXT("10.png"));

		impact[0].Load(TEXT("���� ��¦.png"));
		impact[1].Load(TEXT("��Ȳ ��¦.png"));
		impact[2].Load(TEXT("��� ��¦.png"));
		impact[3].Load(TEXT("���� ��¦.png"));
		impact[4].Load(TEXT("�ʷ� ��¦.png"));
		impact[5].Load(TEXT("�ϴ� ��¦.png"));
		impact[6].Load(TEXT("�Ķ� ��¦.png"));
		impact[7].Load(TEXT("���� ��¦.png"));


		menu[0].Load(TEXT("�޴�.png"));
		menu[1].Load(TEXT("�޴�_������.png"));
		menu[2].Load(TEXT("�޴�_1p.png"));
		menu[3].Load(TEXT("�޴�_2p.png"));

		item_menu[0].Load(TEXT("������ �޴�1.png"));
		item_menu[1].Load(TEXT("������ �޴�2.png"));
		item_menu[2].Load(TEXT("������ �޴�1-1.png"));
		item_menu[3].Load(TEXT("������ �޴�1-2.png"));
		item_menu[4].Load(TEXT("������ �޴�2-1.png"));
		item_menu[5].Load(TEXT("������ �޴�2-2.png"));

		GAME_OVER[0].Load(TEXT("���� ȭ��1.png"));
		GAME_OVER[1].Load(TEXT("���� ȭ��2.png"));

		mx = 235;
		my = 700;
		score = 0;
	

		for (int i = 0; i < MONSTER; i++)//���� ��ǥ �ʱ�ȭ
		{

			bug[i].x = -100;
			bug[i].y = -300;
			bug[i].x_move = 0;
			bug[i].y_move = 0;
			bug[i].state = 1;
		}

		player.x = 235;
		player.y = 650;
		player.w = 30;
		player.h = 30;

		player.picX = 0;
		player.picY = 0;
		player.picW = 76;
		player.picH = 74;

		player.state = 1;

		//
		SecondPlayer.x = 230;
		SecondPlayer.y = 700;
		SecondPlayer.w = 30;
		SecondPlayer.h = 30;

		SecondPlayer.picX = 0;
		SecondPlayer.picY = 0;
		SecondPlayer.picW = 76;
		SecondPlayer.picH = 74;

		SecondPlayer.state = 1;
		//


		backGround1.x = 0;
		backGround1.y = 0;
		backGround1.w = 500;
		backGround1.h = 800;

		backGround1.picX = 0;
		backGround1.picY = 0;
		backGround1.picW = 500;
		backGround1.picH = 800;

		backGround2.x = 0;
		backGround2.y = -800;
		backGround2.w = 500;
		backGround2.h = 800;

		backGround2.picX = 0;
		backGround2.picY = 0;
		backGround2.picW = 500;
		backGround2.picH = 800;

		for (int i = 0; i < 10; ++i)
		{
			//������ 1
			item_1[i].x = player.x - 82;
			item_1[i].y = player.y - 73;
			item_1[i].w = 230;
			item_1[i].h = 220;

			item_1[i].picX = 0;
			item_1[i].picY = 0;
			item_1[i].picW = 230;
			item_1[i].picH = 220;

			//������ 6
			item_6[i].x = player.x - 65;
			item_6[i].y = player.y - 60;
			item_6[i].w = 114;
			item_6[i].h = 122;

			item_6[i].picX = 0;
			item_6[i].picY = 0;
			item_6[i].picW = 114;
			item_6[i].picH = 122;

			//������ 8
			item_8[i].x = player.x - 65;
			item_8[i].y = player.y - 60;
			item_8[i].w = 157;
			item_8[i].h = 150;

			item_8[i].picX = 0;
			item_8[i].picY = 0;
			item_8[i].picW = 157;
			item_8[i].picH = 150;

			//������ 10
			item_10[i].x = player.x;
			item_10[i].y = player.y - 60;
			item_10[i].w = 30;
			item_10[i].h = 30;

			item_10[i].picX = 0;
			item_10[i].picY = 0;
			item_10[i].picW = 76;
			item_10[i].picH = 75;
		}

		break;
	case WM_CHAR:
		switch (wParam)
		{

			if (SecondPlayer.state == 1)
			{
		case 'w':
		case 'W':
			SecondPlayer.y -= 5;
			break;
		case 'a':
		case 'A':
			SecondPlayer.x -= 5;
			break;
		case 's':
		case 'S':
			SecondPlayer.y += 5;
			break;
		case 'd':
		case 'D':
			SecondPlayer.x += 5;
			break;



			}
		case '1':
			ITEM1(item_1[item_1_count], item_1_count);
			//ITEM1(SecondPlayer, item_1[item_1_count], item_1_count);
			item_1_Flag[item_1_count] = 1;
			item_1_count++;
			if (item_1_count == 10)
			{
				item_1_count = 0;
			}
			break;

		case '6':
			ITEM6(item_6[item_6_count], item_6_count);
			item_6_Flag[item_6_count] = 1;
			item_6_count++;
			if (item_6_count == 10)
			{
				item_6_count = 0;
			}
			break;
		case '8':
			ITEM8(player, item_8[item_8_count], item_8_count);
			item_8_Flag[item_8_count] = 1;
			item_8_count++;
			if (item_8_count == 10)
			{
				item_8_count = 0;
			}
			break;

		case '0':
			ITEM10(player, item_10[item_10_count], item_10_count);
			item_10_Flag[item_10_count] = 1;
			item_10_count++;
			if (item_10_count == 10)
			{
				item_10_count = 0;
			}
			break;

		case '2':
			interrupt_ITEM2_Flag = 1;
			break;

		case 'p':
		case 'P':
			if (START)
			{
				if (!pause)
				{
					KillTimer(hWnd, 1);
					pause = true;
				}
				else
				{
					SetTimer(hWnd, 1, 17, NULL);
					pause = false;
				}

			}
			break;

		}
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			//���� Ű
		case VK_SPACE:
			if (START)
			{
				key++;
				key = key % 2;
			}
			break;
		}

		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (!START)
		{
			if (itme_menu_check == 6)
			{

				if (350 < mx&&mx < 500)
				{
					if ((500 < my&&my < 550) && menu_check == 1)
					{
						itme_menu_check = 0;
						FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

					}
					else if ((570 < my&&my < 620) && (menu_check == 2))
					{
						//SecondPlayer.x = -400;
						//SecondPlayer.y = -400;
						//SecondPlayer.state = 0;
						START = true;
						FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);


					}
					else if ((640 < my&&my < 690) && (menu_check == 3))
					{
						START = true;
						mode_2p = true;
						FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

					}

				}
			}
			else
			{
				if (20 < mx&&mx < 120)
				{
					if (20 < my&&my < 70)
					{
						if (itme_menu_check == 2)
						{
							itme_menu_check = 6;
							FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

						}
						else if (itme_menu_check == 4)
						{
							itme_menu_check = 6;
							FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

						}
					}
				}
				if (380 < mx&&mx < 480)
				{
					if (20 < my&&my < 70)
					{
						if (itme_menu_check == 3)
						{
							itme_menu_check = 1;
							FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

						}
						else if (itme_menu_check == 5)
						{
							itme_menu_check = 0;
							FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

						}
					}
				}
			}
		}
		if (Gameover)
		{
			if (180 < mx&&mx < 320)
			{
				if (450 < my&&my < 520)
				{
					FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[12]);

					START = false;
					Gameover = false;
					menu_check = 0;

					SecondPlayer.x = 230;
					SecondPlayer.y = 700;
					player.x = 235;
					player.y = 650;
					score = 0;
					bug_num = 0;
					player.state = 1;
					SecondPlayer.state = 1;
					mode_2p = false;
					for (int i = 0; i < 500; i++)
					{
						bug[i].x = -100;
						bug[i].y = -100;
						bug[i].state = 0;
					}
					item_Drop_Timer = 0;
					item_Count = 0;
					for (int i = 0; i < 20; ++i)
					{
						item_Drop[i].state = 0;
					}
					item_1_count = 0;
					item_6_count = 0;
					item_8_count = 0;
					item_10_count = 0;
					for (int i = 0; i < 10; i++)
					{
						
						/*item_3[i]=false;
						item_4[i]=false;
						item_7[i]=false;
						item_9[i] = false;*/
						item_1_Flag[i] = 0;
						item_6_Flag[i] = 0;
						item_8_Flag[i] = 0;
						item_10_Flag[i] = 0;
					}
					//sp_bug = false;
					//Br_num = 0;
					//Bee_num = 0;
					//Bb_num = 0;
					//Cm_num = 0;
					interrupt_ITEM2_Flag = 0;
					key = 0;
					
				}
			}
		}


		break;
	case WM_LBUTTONUP:


		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		if (!START)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			menu_check = check_menu(mx, my);
		}
		if (itme_menu_check < 6)
		{
			if (itme_menu_check == 0 || itme_menu_check == 2 || itme_menu_check == 3)
			{
				if (20 < my&&my < 70)
				{
					if (20 < mx&&mx < 120)
					{
						itme_menu_check = 2;
					}
					else if (380 < mx&&mx < 480)
					{
						itme_menu_check = 3;
					}
					else
					{
						itme_menu_check = 0;
					}

				}
				else
				{
					itme_menu_check = 0;
				}
			}
			if (itme_menu_check == 1 || itme_menu_check == 4 || itme_menu_check == 5)
			{
				if (20 < my&&my < 70)
				{
					if (20 < mx&&mx < 120)
					{
						itme_menu_check = 4;
					}
					else if (380 < mx&&mx < 480)
					{
						itme_menu_check = 5;
					}
					else
					{
						itme_menu_check = 1;
					}

				}
				else
				{
					itme_menu_check = 1;
				}
			}
		}
			
		
		if (Gameover)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			if (180 < mx&&mx < 320)
			{
				if (450 < my&&my < 520)
				{
					over_check = 1;
				}
				else
				{
					over_check = 0;
				}
			}
		}

		break;
	case WM_TIMER:

		if (START && (!Gameover))
		{
			score++;
			//������ ��� = ���� ����
			item_Drop_Timer++;
			if (item_Drop_Timer % 50 == 0)
			{
				if (item_Drop[item_Count].state == 0)
				{
					item_Drop[item_Count].state = rand() % 10 + 1;
					//item_Drop[item_Count].state = ITEM_TYPE;
					item_Drop[item_Count].x = rand() % 450;
					item_Drop[item_Count].y = -30;
					item_Drop[item_Count].w = 40;
					item_Drop[item_Count].h = 40;
					item_Drop[item_Count].picX = 0;
					item_Drop[item_Count].picY = 0;
					item_Drop[item_Count].picW = 40;
					item_Drop[item_Count].picH = 40;
				}
				//cout << item_Drop[item_Count].state << endl;
				item_Count++;
				if (item_Count >= 20)
					item_Count = 0;
			}

			//������ ��� => �̵� + ������ ��� + �÷��̾� �浹
			for (int i = 0; i < 20; ++i)
			{

				if (item_Drop[i].state != 0)
				{
					item_Drop[i].y += 2;

					if (item_Drop[i].y + item_Drop[i].h >= 800)
						item_Drop[i].state = 0;

					//��� �����۰� �浹üũ
					//1p�϶� 
					if (SecondPlayer.state == 0)
					{
						if (IsItemCollisionCheck(item_Drop[i], player))
						{
							if (item_Drop[i].state == 1)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[1], 0, &g_Channel[1]);

								if (item_1_count == 10)
								{
									item_1_count = 0;
								}
								item_1[item_1_count].x = item_Drop[i].x - 100;
								item_1[item_1_count].y = item_Drop[i].y - 100;

								item_1_Flag[item_1_count] = 1;
								ITEM1(item_1[item_1_count], item_1_count);

								item_1_count++;
								item_Drop[i].state = 0;
							}
							if (item_Drop[i].state == 2)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[2], 0, &g_Channel[2]);

								interrupt_ITEM2_Flag = 1;
								item_Drop[i].state = 0;
							}
						
							if (item_Drop[i].state == 6)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[6], 0, &g_Channel[6]);

								if (item_6_count == 10)
								{
									item_6_count = 0;
								}
								item_6[item_6_count].x = item_Drop[i].x - 50;
								item_6[item_6_count].y = item_Drop[i].y - 50;

								item_6_Flag[item_6_count] = 1;
								ITEM6(item_6[item_6_count], item_6_count);
								item_6_count++;
								item_Drop[i].state = 0;
							}
						
							if (item_Drop[i].state == 8)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[8], 0, &g_Channel[8]);

								if (item_8_count == 10)
								{
									item_8_count = 0;
								}

								item_8[item_8_count].x = player.x - 65;
								item_8[item_8_count].y = player.y - 60;


								item_8_Flag[item_8_count] = 1;
								ITEM8(player, item_8[item_8_count], item_8_count);
								
								item_8[item_8_count].collisionWithWho = 1;
								item_8_count++;
								item_Drop[i].state = 0;
							}
							
							if (item_Drop[i].state == 10)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[10], 0, &g_Channel[10]);

								if (item_10_count == 10)
								{
									item_10_count = 0;
								}
								item_10[item_10_count].x = item_Drop[i].x;
								item_10[item_10_count].y = item_Drop[i].y - 50;

								item_10_Flag[item_10_count] = 1;
								ITEM10(player, item_10[item_10_count], item_10_count);

								item_10[item_10_count].collisionWithWho = 1;

								item_Drop[i].state = 0;
								item_10_count++;
							}
						}
					}
					//2p�϶�
					else
					{
						//1���÷��̾�� �浹
						if (IsItemCollisionCheck(item_Drop[i], player))
						{
							if (item_Drop[i].state == 1)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[1], 0, &g_Channel[1]);

								if (item_1_count == 10)
								{
									item_1_count = 0;
								}
								item_1[item_1_count].x = item_Drop[i].x - 100;
								item_1[item_1_count].y = item_Drop[i].y - 100;

								item_1_Flag[item_1_count] = 1;
								ITEM1(item_1[item_1_count], item_1_count);

								item_1_count++;
								item_Drop[i].state = 0;
							}
							if (item_Drop[i].state == 2)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[2], 0, &g_Channel[2]);

								interrupt_ITEM2_Flag = 1;
								item_Drop[i].state = 0;
							}
						
							if (item_Drop[i].state == 6)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[6], 0, &g_Channel[6]);

								item_6[item_6_count].x = item_Drop[i].x - 50;
								item_6[item_6_count].y = item_Drop[i].y - 50;

								item_6_Flag[item_6_count] = 1;
								ITEM6(item_6[item_6_count], item_6_count);
								
								item_6_count++;
								if (item_6_count == 10)
								{
									item_6_count = 0;
								}
								item_Drop[i].state = 0;
							}
							
							if (item_Drop[i].state == 8)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[8], 0, &g_Channel[8]);

								if (item_8_count == 10)
								{
									item_8_count = 0;
								}
								item_8_Flag[item_8_count] = 1;
								ITEM8(player, item_8[item_8_count], item_8_count);
								
								item_Drop[i].state = 0;

								item_8[item_8_count].collisionWithWho = 1;
								item_8_count++;
							}
						
							if (item_Drop[i].state == 10)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[10], 0, &g_Channel[10]);

								if (item_10_count == 10)
								{
									item_10_count = 0;
								}
								item_10[item_10_count].x = item_Drop[i].x;
								item_10[item_10_count].y = item_Drop[i].y - 50;

								item_10_Flag[item_10_count] = 1;
								ITEM10(player, item_10[item_10_count], item_10_count);
								
								item_Drop[i].state = 0;

								item_10[item_10_count].collisionWithWho = 1;
								item_10_count++;
							}
						}
						//2���÷��̾�� �浹
						if (mode_2p)
						{ 
							if (IsItemCollisionCheck(item_Drop[i], SecondPlayer))
							{
								if (item_Drop[i].state == 1)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[1], 0, &g_Channel[1]);

									item_1[item_1_count].x = item_Drop[i].x - 100;
									item_1[item_1_count].y = item_Drop[i].y - 100;

									item_1_Flag[item_1_count] = 1;
									ITEM1(item_1[item_1_count], item_1_count);

									item_1_count++;
									if (item_1_count == 10)
									{
										item_1_count = 0;
									}
									item_Drop[i].state = 0;
								}
								if (item_Drop[i].state == 2)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[2], 0, &g_Channel[2]);

									interrupt_ITEM2_Flag = 1;
									item_Drop[i].state = 0;
								}
								//if (item_Drop[i].state == 3)
								//{
								//	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[3], 0, &g_Channel[3]);
								//
								//	Br_num++;
								//	if (Br_num == 10)
								//	{
								//		Br_num = 0;
								//	}
								//	Barrier[Br_num].x = SecondPlayer.x - 26;
								//	Barrier[Br_num].y = SecondPlayer.y - 26;
								//
								//	item_3[Br_num] = true;
								//	item_Drop[i].state = 0;
								//
								//	Barrier[Br_num].collisionWithWho = 2;
								//}
								//if (item_Drop[i].state == 4)
								//{
								//	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[4], 0, &g_Channel[4]);
								//
								//	Bee_num++;
								//	if (Bee_num == 10)
								//	{
								//		Bee_num = 0;
								//	}
								//	Bee[Bee_num].x = SecondPlayer.x - 70;
								//	Bee[Bee_num].y = SecondPlayer.y - 70;
								//	item_4[Bee_num] = true;
								//	item_Drop[i].state = 0;
								//
								//	Bee[Bee_num].collisionWithWho = 2;
								//}
								//if (item_Drop[i].state == 5)
								//{
								//	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[5], 0, &g_Channel[5]);
								//
								//}
								if (item_Drop[i].state == 6)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[6], 0, &g_Channel[6]);

									item_6[item_6_count].x = item_Drop[i].x - 50;
									item_6[item_6_count].y = item_Drop[i].y - 50;

									item_6_Flag[item_6_count] = 1;
									ITEM6(item_6[item_6_count], item_6_count);
									
									item_6_count++;
									if (item_6_count == 10)
									{
										item_6_count = 0;
									}
									item_Drop[i].state = 0;
								}
								//if (item_Drop[i].state == 7)
								//{
								//	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[7], 0, &g_Channel[7]);
								//
								//	Bb_num++;
								//
								//	if (Bb_num == 10)
								//	{
								//		Bb_num = 0;
								//	}
								//	for (int i = 0; i < 24; i++) //���̺� ����
								//	{
								//		Baby_bug[Bb_num][i].x = SecondPlayer.x;
								//		Baby_bug[Bb_num][i].y = SecondPlayer.y;
								//		Baby_bug[Bb_num][i].state = 0;
								//	}
								//	bbug_num[Bb_num] = 0;
								//	item_7[Bb_num] = true;
								//	item_Drop[i].state = 0;
								//
								//	Baby_bug[Bb_num][0].collisionWithWho = 2;
								//
								//}
								if (item_Drop[i].state == 8)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[8], 0, &g_Channel[8]);

									if (item_8_count == 10)
									{
										item_8_count = 0;
									}
									item_8_Flag[item_8_count] = 1;
									ITEM8(SecondPlayer, item_8[item_8_count], item_8_count);
									
									item_Drop[i].state = 0;

									item_8[item_8_count].collisionWithWho = 2;
									item_8_count++;
								}
								//if (item_Drop[i].state == 9)
								//{
								//	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[9], 0, &g_Channel[9]);
								//
								//	Cm_num++;
								//	if (Cm_num == 10)
								//	{
								//		Cm_num = 0;
								//	}
								//	CM_line[Cm_num].x = 60;
								//	CM_line[Cm_num].y = 800;
								//	item_9[Cm_num] = true;
								//	item_Drop[i].state = 0;
								//}
								if (item_Drop[i].state == 10)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[10], 0, &g_Channel[10]);

									if (item_10_count == 10)
									{
										item_10_count = 0;
									}
									item_10[item_10_count].x = item_Drop[i].x;
									item_10[item_10_count].y = item_Drop[i].y - 50;

									item_10[item_10_count].x = SecondPlayer.x;
									item_10[item_10_count].y = SecondPlayer.y - 60;

									item_10_Flag[item_10_count] = 1;
									ITEM10(SecondPlayer, item_10[item_10_count], item_10_count);
									
									item_Drop[i].state = 0;

									item_10[item_10_count].collisionWithWho = 2;
									item_10_count++;
								}
							}
						}
					}//
				}
			}

			//���ؿ�� 2 �÷��� on 
			if (interrupt_ITEM2_Flag == 1)
			{
				if (interrupt_ITEM2_timer < 60)
				{
					interrupt_ITEM2_timer++;
					interrupt_ITEM2_frame += 10;
				}
				else if (interrupt_ITEM2_timer >= 60)
				{
					interrupt_ITEM2_frame -= 10;

					if (interrupt_ITEM2_frame <= 0)
					{
						interrupt_ITEM2_Flag = 0;
						interrupt_ITEM2_timer = 0;
					}
				}
			}


			//�÷��̾� �浹üũ
			//1p
			playerCollisionCheck(player);
			//2p
			if (SecondPlayer.state == 1)
				playerCollisionCheck(SecondPlayer);


			//������ �浹üũ
			for (int i = 0; i < 10; ++i)
			{
				if (item_1_Flag[i] != 0)
					collisionCheck(item_1[i]);
				if (item_6_Flag[i] != 0)
					collisionCheck(item_6[i]);
				if (item_8_Flag[i] != 0)
					collisionCheck(item_8[i]);
				if (item_10_Flag[i] != 0)
					collisionCheck(item_10[i]);
			}

			//�÷��̾� �����̱�
			distance = sqrt(pow(mx - 15 - player.x, 2) + pow(my - 15 - player.y, 2));
			rotation = atan2(mx - 15 - player.x, my - 15 - player.y);


			if (distance > 15)
			{
				player.x += sin(rotation) * speed;
				player.y += cos(rotation) * speed;
			}

			switch (wParam)// ��� �����̱�
			{
			case 1:
				//2p�̵�
				if (SecondPlayer.state == 1)
				{
					if (GetKeyState(VK_LEFT) & 0x8000)      SecondPlayer.x -= 5;
					if (GetKeyState(VK_RIGHT) & 0x8000)     SecondPlayer.x += 5;
					if (GetKeyState(VK_UP) & 0x8000)        SecondPlayer.y -= 5;
					if (GetKeyState(VK_DOWN) & 0x8000)      SecondPlayer.y += 5;
				}





				backGround1.y += 5;
				backGround2.y += 5;

				if (backGround1.y >= 800)
				{
					backGround1.y = -800;
				}
				if (backGround2.y >= 800)
				{
					backGround2.y = -800;
				}
			}

			//������ �÷��� Ȯ��
			for (int i = 0; i < 10; ++i)
			{
				if (score % 2 == 0)
				{
					//������ 1 �÷���
					if (item_1_Flag[i] == 1)
					{
						ITEM1(item_1[i], i);
					}
		
					//������ 6 �÷���
					if (item_6_Flag[i] != 0)
					{
						ITEM6(item_6[i], i);
					}

					//������ 8 �÷���
					if (item_8_Flag[i] != 0)
					{
						if (item_8[i].collisionWithWho == 1)
							ITEM8(player, item_8[i], i);
						if (item_8[i].collisionWithWho == 2)
							ITEM8(SecondPlayer, item_8[i], i);
					}

					//������ 10 �÷���
					if (item_10_Flag[i] != 0)
					{
						if (item_10[i].collisionWithWho == 1)
							ITEM10(player, item_10[i], i);
						if (item_10[i].collisionWithWho == 2)
							ITEM10(SecondPlayer, item_10[i], i);
					}
				}

			}
			//srand((unsigned)time(NULL));
			bug[bug_num].x = (rand() % 500);//���� x��ǥ
			bug[bug_num].y = -50;//���� y��ǥ
			bug[bug_num].y_move = (rand() % 3) + 2;//���� y��ǥ �̵���

			for (int i = 0; i < 10; ++i)
			{
				if (item_10_Flag[i] == 0)
				{
					if (mode_2p)
					{
						if (bug_num % 2 == 0)
						{
							bug[bug_num].x_move = bug_x_move(bug[bug_num], player);// ������ x�̵��� ���ϱ�
						}
						else
						{
							bug[bug_num].x_move = bug_x_move(bug[bug_num], SecondPlayer);// ������ x�̵��� ���ϱ�
						}

					}
					else
					{
						bug[bug_num].x_move = bug_x_move(bug[bug_num], player);// ������ x�̵��� ���ϱ�
					}
				}

				if (item_10_Flag[i] == 1)
				{
					bug[bug_num].x_move = bug_x_move(bug[bug_num], item_10[i]);// ������ x�̵��� ���ϱ�
				}
			}


			
			
			


			bug[bug_num].state = 1;
			bug[bug_num].w = 30;
			bug[bug_num].h = 30;

			if (bug_num == MONSTER - 1)// ���� ���� ����
			{
				bug_num = 0;
			}
			else
			{

				if (rand() % 4 == 0)
				{
					bug_num++;//���� �� ����
				}
			}
			for (int i = 0; i < MONSTER; i++)// ���� �����̱�
			{
				bug[i].x += bug[i].x_move;
				bug[i].y += bug[i].y_move;

				if (bug[i].y + 30 >= 800)
				{
					bug[i].state = 1;
				}
			}
			if (player.state == 0)
			{
				player.x = -500;
				player.y = -500;
			}
			if (SecondPlayer.state == 0)
			{
				SecondPlayer.x = -500;
				SecondPlayer.x = -500;
			}
			if (player.state == 0)
			{
				if (mode_2p)
				{
					if (SecondPlayer.state == 0)
					{
						Gameover = true;
					}
				}
				else
				{
					Gameover = true;
				
				}

			}
		}

		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hDC);

		hBit = CreateCompatibleBitmap(hDC, 1000, 800);
		SelectObject(memdc, hBit);




		//��� �׸�
		//
		BACKGROUND.Draw(memdc, backGround1.x, backGround1.y, backGround1.w, backGround1.h, backGround1.picX, backGround1.picY, backGround1.picW, backGround1.picH);
		BACKGROUND.Draw(memdc, backGround2.x, backGround2.y, backGround2.w, backGround2.h, backGround2.picX, backGround2.picY, backGround2.picW, backGround2.picH);

		//������ �׸���
		for (int i = 0; i < 10; ++i)
		{
			//������ 1
			if (item_1_Flag[i] == 1)
			{
				ITEM_1.Draw(memdc, item_1[i].x, item_1[i].y, item_1[i].w, item_1[i].h, item_1[i].picX, item_1[i].picY, item_1[i].picW, item_1[i].picH);
			}

			//������ 6
			if (item_6_Flag[i] == 1)
			{
				ITEM_6.Draw(memdc, item_6[i].x, item_6[i].y, item_6[i].w, item_6[i].h, item_6[i].picX, item_6[i].picY, item_6[i].picW, item_6[i].picH);
			}

			//������ 8
			if (item_8_Flag[i] != 0)
			{
				if (item_8[i].collisionWithWho == 1)
				{
					//�ֺ� �ɵ� ��
					if (item_8_Flag[i] == 1)
						ITEM_8.Draw(memdc, player.x - 65, player.y - 60, item_8[i].w, item_8[i].h, item_8[i].picX, item_8[i].picY, item_8[i].picW, item_8[i].picH);
					//������ ������ ��
					if (item_8_Flag[i] == 2)
						ITEM_8.Draw(memdc, item_8[i].x, item_8[i].y, item_8[i].w, item_8[i].h, item_8[i].picX, item_8[i].picY, item_8[i].picW, item_8[i].picH);
				}
				else
				{
					//�ֺ� �ɵ� ��
					if (item_8_Flag[i] == 1)
						ITEM_8.Draw(memdc, SecondPlayer.x - 65, SecondPlayer.y - 60, item_8[i].w, item_8[i].h, item_8[i].picX, item_8[i].picY, item_8[i].picW, item_8[i].picH);
					//������ ������ ��
					if (item_8_Flag[i] == 2)
						ITEM_8.Draw(memdc, item_8[i].x, item_8[i].y, item_8[i].w, item_8[i].h, item_8[i].picX, item_8[i].picY, item_8[i].picW, item_8[i].picH);

				}
			}

			//������ 10
			if (item_10_Flag[i] != 0)
			{
				ITEM_10.Draw(memdc, item_10[i].x, item_10[i].y, item_10[i].w, item_10[i].h, item_10[i].picX, item_10[i].picY, item_10[i].picW, item_10[i].picH);
			}
		}

		//�÷��̾�
		if (player.state == 1)
		{
			PLAYER.TransparentBlt(memdc, player.x, player.y, player.w, player.h, player.picX, player.picY, player.picW, player.picH, RGB(255, 255, 255));//�÷��̾�
		}
		if (mode_2p)
		{
			if (SecondPlayer.state == 1)
			{
				SECONDPLAYER.Draw(memdc, SecondPlayer.x, SecondPlayer.y, SecondPlayer.w, SecondPlayer.h, SecondPlayer.picX, SecondPlayer.picY, SecondPlayer.picW, SecondPlayer.picH);//�÷��̾�
			}
		}

		//����
		for (int i = 0; i < MONSTER; i++)
		{

			if (bug[i].state == 1)
			{
				BUG.TransparentBlt(memdc, bug[i].x, bug[i].y, player.w, player.h, player.picX, player.picY, 72, 73, RGB(255, 255, 255));
			}
			if (bug[i].impact_num != 9)
			{

				impact[bug[i].impact_num].Draw(memdc, bug[i].x - 15, bug[i].y - 15, player.w + 15, player.h + 15, bug[i].impact_time * 172, 0, 172, 160);
				bug[i].impact_time++;
				if (bug[i].impact_time == 4)
				{
					bug[i].impact_time = 0;
					bug[i].impact_num = 9;
				}
			}
		}

		//������ ���
		for (int i = 0; i < 20; ++i)
		{
			//Rectangle(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].x + item_Drop[i].w, item_Drop[i].y + item_Drop[i].h);

			if (item_Drop[i].state == 1)
				image1.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 2)
				INTERRUPT_ITEM2.Draw(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH);
			if (item_Drop[i].state == 3)
				image3.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 4)
				image4.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 5)
				//INTERRUPT_ITEM1.Draw(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH);
			if (item_Drop[i].state == 6)
				image6.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 7)
				image7.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 8)
				image8.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 9)
				image9.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));
			if (item_Drop[i].state == 10)
				image10.TransparentBlt(memdc, item_Drop[i].x, item_Drop[i].y, item_Drop[i].w, item_Drop[i].h, item_Drop[i].picX, item_Drop[i].picY, item_Drop[i].picW, item_Drop[i].picH, RGB(0, 255, 255));

		}


		//���ؿ�� 2
		if (interrupt_ITEM2_Flag == 1)
		{
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			oldBrush = ((HBRUSH)SelectObject(memdc, hBrush));
			Rectangle(memdc, 0, 0, 500, interrupt_ITEM2_frame);
			SelectObject(memdc, oldBrush);
			DeleteObject(hBrush);
		}




		//����
		if (Gameover)
		{
			GAME_OVER[over_check].Draw(memdc, 0, 0, 500, 800, 0, 0, 606, 1080);
			//Rectangle(memdc, 180, 450, 320, 520);
			//SetTextColor(memdc, RGB(255, 228, 0));
			SetBkMode(memdc, TRANSPARENT);
			myFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("���"));
			oldFont = (HFONT)SelectObject(memdc, myFont);
			wsprintf(str, L"%d  ", score);
			TextOut(memdc, 200, 385, str, lstrlen(str));
			SelectObject(memdc, oldFont);
			DeleteObject(myFont);
		}
		else
		{
			SetTextColor(memdc, RGB(255, 228, 0));
			SetBkMode(memdc, TRANSPARENT);
			myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("���"));
			oldFont = (HFONT)SelectObject(memdc, myFont);
			wsprintf(str, L"SCORE : %d  ", score);
			TextOut(memdc, 10, 10, str, lstrlen(str));
			if (key == 1)
			{
				wsprintf(str, L"���� ");
				TextOut(memdc, 400, 10, str, lstrlen(str));
			}
			SelectObject(memdc, oldFont);
			DeleteObject(myFont);
		}

		//cout << sp_bug_time << endl;

		if (pause)
		{
			SetTextColor(memdc, RGB(255, 255, 255));
			SetBkMode(memdc, TRANSPARENT);
			myFont = CreateFont(80, 00, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("���"));
			oldFont = (HFONT)SelectObject(memdc, myFont);
			wsprintf(str, L"PAUSE  ", score);
			TextOut(memdc, 110, 200, str, lstrlen(str));
			SelectObject(memdc, oldFont);
			DeleteObject(myFont);
		}
		if (!START)
		{
			//cout << mx << my << endl;
			menu[menu_check].Draw(memdc, 0, 0, 500, 800, 0, 0, 596, 996);
		}
		if (itme_menu_check < 6)
		{
			item_menu[itme_menu_check].Draw(memdc, 0, 0, 500, 800, 0, 0, 500, 900);

		}
		BitBlt(hDC, 0, 0, 500, 800, memdc, 0, 0, SRCCOPY);

		DeleteObject(hBit);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		Release();
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}



//������ 1
void ITEM1(Move &item_1, int i)
{

	item_1.picX = 0 + (item_1_frame[i] * 230);
	item_1.picY = item_1_carry[i] * 226;
	item_1_frame[i]++;

	if (item_1_frame[i] >= 8)
	{
		item_1_frame[i] = 0;
		item_1_carry[i]++;
		//item_1_Flag = 0;
	}
	if (item_1_carry[i] > 5)
	{
		item_1_frame[i] = 0;
		item_1_carry[i] = 0;
		item_1_Flag[i] = 0;
	}
}

//������ 6
void ITEM6(Move &item_6, int i)
{
	item_6.picX = 0 + (item_6_frame[i] * 114);
	item_6_frame[i]++;
	ITEM6_MOVE(item_6, i);
	if (item_6_frame[i] >= 5)
	{
		item_6_frame[i] = 0;
	}
	if (item_6_bounce[i] >= 6)
	{
		item_6_bounce[i] = 0;
		item_6_Flag[i] = 0;
		item_6_frame[i] = 0;
		item_6_direction[i] = 0;
	}
}
void ITEM6_DIRECTION(Move &item_6, int i)
{
	if (item_6_direction[i] == 0)
	{
		if (item_6.y <= 0)
		{
			item_6_direction[i] = 4;
			item_6_bounce[i]++;
		}
	}
	if (item_6_direction[i] == 4)
	{
		if (item_6.x <= 0)
		{
			item_6_direction[i] = 3;
			item_6_bounce[i]++;
		}
		else if (item_6.y + item_6.h >= 800)
		{
			item_6_direction[i] = 1;
			item_6_bounce[i]++;
		}
	}
	else if (item_6_direction[i] == 3)
	{
		if (item_6.x + item_6.w >= 500)
		{
			item_6_direction[i] = 4;
			item_6_bounce[i]++;
		}
		else if (item_6.y + item_6.h >= 800)
		{
			item_6_direction[i] = 2;
			item_6_bounce[i]++;
		}
	}
	else if (item_6_direction[i] == 2)
	{
		if (item_6.x + item_6.w >= 500)
		{
			item_6_direction[i] = 1;
			item_6_bounce[i]++;
		}
		else if (item_6.y <= 0)
		{
			item_6_direction[i] = 3;
			item_6_bounce[i]++;
		}
	}
	else if (item_6_direction[i] == 1)
	{
		if (item_6.x <= 0)
		{
			item_6_direction[i] = 2;
			item_6_bounce[i]++;
		}
		else if (item_6.y <= 0)
		{
			item_6_direction[i] = 4;
			item_6_bounce[i]++;
		}
	}

}
void ITEM6_MOVE(Move &item_6, int i)
{
	if (item_6_direction[i] == 0)
	{
		item_6.y -= 13;
		ITEM6_DIRECTION(item_6, i);
	}
	else if (item_6_direction[i] == 1)
	{
		item_6.x -= 13;
		item_6.y -= 13;
		ITEM6_DIRECTION(item_6, i);
	}
	else if (item_6_direction[i] == 2)
	{
		item_6.x += 13;
		item_6.y -= 13;
		ITEM6_DIRECTION(item_6, i);
	}
	else if (item_6_direction[i] == 3)
	{
		item_6.x += 13;
		item_6.y += 13;
		ITEM6_DIRECTION(item_6, i);
	}
	else if (item_6_direction[i] == 4)
	{
		item_6.x -= 13;
		item_6.y += 13;
		ITEM6_DIRECTION(item_6, i);
	}
}

//������ 8
void ITEM8(Move &player, Move &item_8, int i)
{
	item_8.picX = 0 + (item_8_frame[i] * 157);
	item_8_frame[i]++;
	//item_8.picX = 0 + (item_8[i].count * 157);
	//item_8_frame[i]++;
	//cout << i << " : " << item_8_frame[i] << endl;

	if (item_8_frame[i] >= 7)
	{
		item_8_frame[i] = 0;

		//cout << i << " :: " << item_8_frame[i] << endl;

		if (item_8_Flag[i] != 2)
		{
			//cout << i << " ::: " << item_8_frame[i] << endl;

			item_8_Flag[i] = 2;//������ ���� �÷��� on
			item_8.x = player.x - 65;
			item_8.y = player.y - 60;
		}
	}

	//������ ����
	if (item_8_Flag[i] == 2)
	{
		item_8.y -= 7;

		//ȭ�鿡�� �Ⱥ������� �÷��� ����
		if (item_8.y <= -300)
		{
			item_8_Flag[i] = 0;
		}
	}
}

//������ 10
void ITEM10(Move &player, Move &item_10, int i)
{

	item_10.y -= 5;

	if (item_10.y + item_10.h <= 0)
	{
		item_10_Flag[i] = 0;
	}
}