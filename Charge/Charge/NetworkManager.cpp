#include "common.h"
#include "protocol.h"
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
	if ( INVALID_SOCKET == m_ClientSocket )
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
	while ( /*THREAD_MAX_COUNT*/100 > m_UserCount )
	{
		SOCKET con = accept(m_ClientSocket, (sockaddr*)&addr, &addrLen);

		//SC_ID_PUT_PACKET idPacket(m_UserCount);
		//send(con, (const char*)&idPacket, sizeof(m_UserCount),0);


		Player* player = new Player;
		Session* session = new Session(con, addr, player, m_UserCount);
		CreateThread( session );
	}

	for ( auto& i : m_Threads )
	{
		i.join();
	}

	
}

void NetworkManager::CreateThread( Session* sesion )
{
	// ㄷㄱㅎ
	m_Threads.push_back(std::thread{ WorkerThread, sesion });
	AddUserCount();
}

void NetworkManager::WorkerThread( Session* session )
{
	// send();
	int ret = 0;

	while (true)
	{


		// input
		//char buffer[512]{ "사람" };
		//char buffer2[512]{ 0 };
		//printf_s( "입력 : " );
		//std::cin>> buffer;
		//ret = send(session->clientSocket, buffer, strlen(buffer), 0);
		//printf_s("송신 완료 : \n");
		
		//ret = recv(session->clientSocket, buffer, 512, 0);
		//printf_s("수신 완료 %s \n", buffer);
		//buffer[ret] = NULL;
		

		// FlatBuffer


		//for (auto i = msg->begin(); i != msg->end() ;i++ )
		//{
		//	printf_s("%c", (*i) );
		//}

	}

}
