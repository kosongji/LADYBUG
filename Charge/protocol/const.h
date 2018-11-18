#pragma once
#include "common.h"

#define THREAD_MAX_COUNT 2

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

//struct Sync
//{
//
//	Player* player;
//	//Bug*	bugs;
//	//Item*	Items;
//};
