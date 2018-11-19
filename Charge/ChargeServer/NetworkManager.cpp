#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"
#include "../protocol/Singleton.h"

#include "ObjectManager.h"
#include "NetworkManager.h"

#include <random>

namespace CS
{
	NetworkManager* GNetworkManager = new NetworkManager();
}



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
		printf_s(" ERROR ! CS::NetworkManager::WSAStratup() ");
		return false;
	}
	
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( INVALID_SOCKET == m_ClientSocket)
	{
		printf_s(" ERROR ! CS::NetworkManager::Initialize() ");
		return false;
	}

	SOCKADDR_IN addr{ AF_INET, htons(MY_PORT), htonl(INADDR_ANY) };
	if ( SOCKET_ERROR == ::bind(m_ClientSocket, (SOCKADDR*)&addr, sizeof(addr)) )
	{
		printf_s(" ERROR ! CS::NetworkManager::Initialize() ");
		return false;
	}

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

void CS::NetworkManager::AcceptLoop()
{

	if (SOCKET_ERROR == listen(m_ClientSocket, SOMAXCONN))
	{
		printf_s("[DEBUG] listen error\n");
		return;
	}

	SOCKADDR_IN addr { 0, };
	int addrLen = sizeof(addr);
	while ( THREAD_MAX_COUNT > m_UserCount )
	{
		SOCKET con = accept(m_ClientSocket, (sockaddr*)&addr, &addrLen);

		Session* sess = new Session( con, addr, m_UserCount);
		AddUserCount();
		CreateThread( sess );
		
	}



}

void CS::NetworkManager::CreateThread( Session* sesion )
{
	m_Threads.push_back(std::thread{ WorkerThread, sesion, this });
}

void CS::NetworkManager::WorkerThread( Session* session, NetworkManager* ppp )
{
	NetworkManager* pp = ppp;
	std::uniform_int_distribution<> uid;
	int ret = 0;
	char buffer[1024];

	float f = 1.0;
	

SC_ID_PUT_PACKET packet(pp->m_UserCount);
int n2 = send(session->clientSocket, (char*)&packet, sizeof(SC_ID_PUT_PACKET), 0);

if (SOCKET_ERROR == n2)
{

	printf_s("Client send error %d\n", WSAGetLastError());
}
else
{
	printf_s("ID : Client send\n");
}

	while (true)
	{
		Sleep(33);

		int n2 = recv(session->clientSocket, buffer, sizeof(buffer), 0);
		if (SOCKET_ERROR == n2)
		{
			printf_s("Recv error\n");
			break;
		}
		else
		{
			buffer[n2] = NULL;
			printf_s("recv %s\n", buffer);
		}

		pp->OnProcessPakcet(buffer);

	
		SC_PLAYER_MOVE_PACKET p(11 , 1.0 +f , 0.0 +f);
		int n = send( session->clientSocket, (char*)&p, p.length, 0);
		if (SOCKET_ERROR == n)
		{
			printf_s("server error\n");
			break;
		}
		else
		{
			printf_s("send\n");
		}
		
		f += 1.0;

	}

}

void CS::NetworkManager::OnProcessPakcet(char* buf)
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

	case CS_MOVE_DOWN:
		printf_s("SC_MOVE_OBJ\n");
		printf_s("아래로\n");

		break;
	case CS_MOVE_UP:
		printf_s("SC_MOVE_OBJ\n");
		printf_s("위로\n");


		break;
	case CS_MOVE_LEFT:
		printf_s("SC_MOVE_OBJ\n");
		printf_s("왼\n");

		/*memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));

		printf_s("x : %.6f \n", x);
		printf_s("y : %.6f \n", y);*/

	case CS_MOVE_RIGHT:
		printf_s("SC_MOVE_OBJ\n");
		printf_s("오른\n");
		
		//memcpy(&x, buf + 3, sizeof(float));
		//memcpy(&y, buf + 7, sizeof(float));

		//printf_s("x : %.6f \n", x);
		//printf_s("y : %.6f \n", y);

		break;
	case CS_GAME_DISCONNECT:
		printf_s("SC_MOVE_OBJ\n");

		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));

		printf_s("x : %.6f \n", x);
		printf_s("y : %.6f \n", y);

		break;
	}



}

