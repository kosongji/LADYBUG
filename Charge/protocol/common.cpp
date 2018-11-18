#include "protocol.h"
#include "common.h"

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

short processPckaetClient(char *buf)	
{
	char buffer[1024] {0};
	memcpy(buffer, buf, buf[0]);
	short size = buf[0];
	char type = buf[2];
	float x;
	float y;

	switch (type)
	{
	
	case SC_ID_PUT:
		printf_s("SC_ID_PUT\n");
		break;

	case SC_GAME_INIT_INFO :
		printf_s("SC_GAME_INIT_INFO\n");
		break;

	case SC_GAME_START:
		printf_s("SC_GAME_START\n");
		break;

	case SC_MOVE_DOWN :
		printf_s("SC_MOVE_OBJ\n");
		
		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));		
		//x = buf[3];
		//y = buf[7];
		printf_s("x : %.6f \n", x/*(float)buf[3]*/);
		printf_s("y : %.6f \n", y/*(float)buf[7]*/);		

	case SC_MOVE_UP:
		printf_s("SC_MOVE_OBJ\n");

		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));
		//x = buf[3];
		//y = buf[7];
		printf_s("x : %.6f \n", x/*(float)buf[3]*/);
		printf_s("y : %.6f \n", y/*(float)buf[7]*/);

	case SC_MOVE_LEFT:
		printf_s("SC_MOVE_OBJ\n");

		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));
		//x = buf[3];
		//y = buf[7];
		printf_s("x : %.6f \n", x/*(float)buf[3]*/);
		printf_s("y : %.6f \n", y/*(float)buf[7]*/);

	case SC_MOVE_RIGHT:
		printf_s("SC_MOVE_OBJ\n");

		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));
		//x = buf[3];
		//y = buf[7];
		printf_s("x : %.6f \n", x/*(float)buf[3]*/);
		printf_s("y : %.6f \n", y/*(float)buf[7]*/);


		break; 
	case SC_USE_ITEM :
		printf_s("SC_USE_ITEM\n");
		break; 
	case SC_DEAD :
		printf_s("SC_DEAD\n");
		break; 
	case SC_GAME_END :
		printf_s("SC_GAME_END\n");
		break;
	}

	return 0;
}

short processPckaetServer(char *buf)
{
	char buffer[1024]{ 0 };
	memcpy(buffer, buf, buf[0]);
	short size = buf[0];
	char type = buf[2];
	float x;
	float y;

	switch (type)
	{

	case CS_READY:
		printf_s("CS_READY\n");
		break;
	case CS_UNREADY:
		printf_s("CS_UNREADY\n");
		break;
	case CS_INIT_COMPLETE:
		printf_s("CS_INIT_COMPLETE\n");

		break;
	case CS_MOVE:
		printf_s("CS_MOVE\n");
		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));

		printf_s("X : %.6f \n", x);
		printf_s("Y : %.6f \n", y);
		break;
	case CS_GAME_DISCONNECT:
		printf_s("CS_GAME_DISCONNECT\n");
		break;

	}

	return 0;
}

void gotoxy(int x, int y)
{

	COORD Pos = { x - 1, y - 1 };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

}