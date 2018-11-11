#pragma once
#include "common.h"
#include "Player.h"

#define THREAD_MAX_COUNT 2

struct Session
{
	SOCKET		clientSocket;
	SOCKADDR_IN	addr;
	Player*	player;
	LONG		id;

	Session(SOCKET sock, SOCKADDR_IN add, Player* pla, LONG id)
		: clientSocket(sock)
	{
	}

};
