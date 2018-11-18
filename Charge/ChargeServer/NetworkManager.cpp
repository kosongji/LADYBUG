#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"
#include "../protocol/Singleton.h"

#include "NetworkManager.h"


NetworkManager::NetworkManager()
	: m_ClientSocket(INVALID_SOCKET), m_UserCount(0)
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::Initialize()
{
	WSADATA wsa;
	if ( 0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		printf_s(" ERROR ! NetworkManager::WSAStratup() ");
		return false;
	}
	
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( INVALID_SOCKET == m_ClientSocket)
	{
		printf_s(" ERROR ! NetworkManager::Initialize() ");
		return false;
	}

	SOCKADDR_IN addr{ AF_INET, htons(MY_PORT), htonl(INADDR_ANY) };
	if ( SOCKET_ERROR == ::bind(m_ClientSocket, (SOCKADDR*)&addr, sizeof(addr)) )
	{
		printf_s(" ERROR ! NetworkManager::Initialize() ");
		return false;
	}

	return true;
}

void NetworkManager::Finalize()
{
	for (auto& th : m_Threads)
	{
		th.join();
	}

	closesocket(m_ClientSocket);
	WSACleanup();
}

void NetworkManager::AcceptLoop()
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

void NetworkManager::CreateThread( Session* sesion )
{
	// ¤§¤¡¤¾
	m_Threads.push_back(std::thread{ WorkerThread, sesion});
	AddUserCount();
}

void NetworkManager::WorkerThread( Session* session )
{

	//std::uniform_int_distribution<> uid;
	int ret = 0;
	char buffer[1024];

	float f = 1.0;
	while (true)
	{
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

		processPckaetServer(buffer);

		PLAYER_MOVE_PACKET p( SC_MOVE_UP,111.233 +f , 123.432134 +f);
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
