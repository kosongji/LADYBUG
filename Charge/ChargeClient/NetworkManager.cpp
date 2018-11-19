#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"
#include "../protocol/Singleton.h"

#include "ObjectManager.h"
#include "NetworkManager.h"
#include <random>

namespace CS
{
	NetworkManager*  GNetworkManager = nullptr;
}

 CS::NetworkManager::NetworkManager()
	: m_ClientSocket(INVALID_SOCKET), m_UserCount(0)
{
}

 CS::NetworkManager::~NetworkManager()
{
}

bool  CS::NetworkManager::Initialize()
{
	WSADATA wsa;
	if ( 0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		printf_s(" ERROR ! CS::NetworkManager::WSAStratup() ");
		return false;
	}
	
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( INVALID_SOCKET == m_ClientSocket )
	{
		printf_s(" ERROR ! CS::NetworkManager::Initialize() ");
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
		printf_s(" ERROR ! CS::NetworkManager::Initialize() ");
	}




	printf_s(" 접속.... \n");
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));




	

	int ret = 0;

	

	return true;
}


void  CS::NetworkManager::Finalize()
{
	for (auto& th : m_Threads)
	{
		th.join();
	}

	closesocket(m_ClientSocket);
	WSACleanup();
}

void  CS::NetworkManager::CreateThread()
{
	m_Threads.push_back(std::thread{ WorkerThread, this });
	AddUserCount();
}



void  CS::NetworkManager::WorkerThread( NetworkManager* pp)
{

	char buffer[1024]{ 0 };

	int n = recv(pp->m_ClientSocket, buffer, 4, 0);
	pp->m_Id = buffer[3];


	std::uniform_int_distribution<> uid(4, 7);
	std::default_random_engine dre;


	
	while (true)
	{
		Sleep(33);
		CS_MOVE_PACKET p( uid(dre) , pp->m_Id);

		int n2 = send(pp->m_ClientSocket, (char*)&p, p.length, 0);
		if (SOCKET_ERROR == n2)
		{

			printf_s("Client send error %d\n", WSAGetLastError());
			break;
		}
		else
		{
			printf_s("ID : %d Client send\n", pp->m_Id);
		}


		int n = recv(pp->m_ClientSocket, buffer, 11, 0);
		if (SOCKET_ERROR == n)
		{
			printf_s("%d\n", WSAGetLastError());
			printf_s("Client Recv error\n");
			break;
		}
		else
		{
			printf_s("%d\n", WSAGetLastError());
			printf_s("Client recv \n");
		}

		pp->OnProcessPakcet(buffer);
	}

}

void  CS::NetworkManager::OnProcessPakcet(char *buf )
{
	char buffer[1024]{ 0 };
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

	case SC_GAME_INIT_INFO:
		printf_s("SC_GAME_INIT_INFO\n");
		break;

	case SC_GAME_START:
		printf_s("SC_GAME_START\n");
		break;

	case SC_MOVE_OBJ:
		// g객체 
		printf_s("CS_MOVE\n");

		memcpy(&x, buf + 3, sizeof(float));
		memcpy(&y, buf + 7, sizeof(float));

		printf_s("X : %.6f \n", x);
		printf_s("Y : %.6f \n", y);
		break;


	case SC_USE_ITEM:
		printf_s("SC_USE_ITEM\n");
		break;

	case SC_DEAD:
		printf_s("SC_DEAD\n");
		break;

	case SC_GAME_END:
		printf_s("SC_GAME_END\n");
		break;
	}


}

bool  CS::NetworkManager::OnSend()
{
	return false;
}

bool  CS::NetworkManager::OnRecv()
{
	return false;
}


