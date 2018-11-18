#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"
#include "../protocol/Singleton.h"


#include "NetworkManager.h"



CS::NetworkManager::NetworkManager()
	: m_ClientSocket(INVALID_SOCKET), m_UserCount(0)
{
}

CS::NetworkManager::~NetworkManager()
{
}

bool CS::NetworkManager::Initialize()
{

	WSADATA wsa;
	if ( 0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		printf_s(" ERROR ! NetworkManager::WSAStratup() ");
		return false;
	}
	
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( INVALID_SOCKET == m_ClientSocket )
	{
		printf_s(" ERROR ! NetworkManager::Initialize() ");
		return false;
	}

	SOCKADDR_IN serveraddr{0,};
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(MY_PORT);

	int conntectlen{ sizeof(serveraddr) };

	int retval;
	retval = connect(m_ClientSocket, (SOCKADDR *)&serveraddr, conntectlen);
	if (retval == SOCKET_ERROR)
	{
		printf_s(" ERROR ! NetworkManager::Initialize() ");
	}

	printf_s(" 접속.... \n");
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));


	

	int ret = 0;

	

	return true;
}


void CS::NetworkManager::Finalize()
{
	for (auto& th : m_Threads)
	{
		th.join();
	}

	closesocket(m_ClientSocket);
	WSACleanup();
}

void CS::NetworkManager::CreateThread()
{
	m_Threads.push_back(std::thread{ WorkerThread, this });
	AddUserCount();
}



void CS::NetworkManager::WorkerThread(CS::NetworkManager* pp)
{
	NetworkManager* netMgr = pp;
	char buffer[1024]{ 0 };

	// iD 받기


	while (true)
	{

		PLAYER_MOVE_PACKET p(SC_MOVE_LEFT, 1.1, 2.2);

		int n2 = send(netMgr->m_ClientSocket, (char*)&p, p.length, 0);
		if (SOCKET_ERROR == n2)
		{
			printf_s("Client send error\n");
			break;
		}
		else
		{
			printf_s("Client send\n");
		}

		int n = recv(netMgr->m_ClientSocket, buffer, 11, 0);
		if (SOCKET_ERROR == n)
		{
			printf_s("Client Recv error\n");
			break;
		}
		else
		{
			printf_s("Client recv \n");
		}

		netMgr->OnProcessPakcet(buffer);

		

	}



}

void CS::NetworkManager::OnProcessPakcet(char *buf )
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


}


//void CS::NetworkManager::Rendering(float x, float y)
//{
//	gotoxy(m_Player[0].x, m_Player[1].y);
//	printf("BUG");
//}
