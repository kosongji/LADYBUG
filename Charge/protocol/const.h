#pragma once
#include "common.h"

#define THREAD_MAX_COUNT 2
#define PLAYER_NUM 2
#define BUG_NUM 10
#define ITEM_NUM 5

struct Session
{
	SOCKET		clientSocket;
	SOCKADDR_IN	addr;
	LONG		id;

	Session(SOCKET sock, SOCKADDR_IN add, LONG id)
		: clientSocket(sock), addr(add), id(id)
	{
	}

};


//struct OnSync
//{
//
//	Player* player;
//	//Bug*	bugs;
//	//Item*	Items;
//};


#pragma pack(push, 1)

struct Object
{
	float x, y;
	Object()
		: x(0.0), y(0.0)
	{
	}
};

struct BUG_OBJ_POS
{
	// 어차피 분기 필요 
	// status 제거
	// 0,0 죽음
	Object pos;
};
struct ITEM_OBJ_POS
{
	unsigned char type;
	Object pos;
};

struct PLAYER_OBJ_POS
{
	Object pos;
};

#pragma pack(pop, 1)

