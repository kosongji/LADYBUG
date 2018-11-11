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
	// send();
	while (true)
	{
		//char buffer[1024]{ 0 };
		//char buffer2[1024]{ 0 };
		//
		//ret = recv(m_ClientSocket, (char*) buffer2, 1024, 0);
		//printf_s("수신 완료 %s \n", buffer2);
		//if (SOCKET_ERROR != ret || 0 == ret)
		//{
		//	std::cout << WSAGetLastError() << std::endl;
		//	std::cout << "흠" << std::endl;
		//}
		//buffer2[ret] = NULL;

		//printf_s("입력 : ");
		//std::cin >> buffer;
		//ret = send(m_ClientSocket, buffer, strlen(buffer), 0);
		//printf_s("송신 완료 : \n");


		//char str[100]{ 0 };
		//strcpy(str, msg->c_str());
		//printf_s("%s \n", msg->c_str());
		
		//const char* c = msg->c_str();

		printf_s("송신 완료 : \n");
	}

	return true;
}

void NetworkManager::Finalize()
{
	closesocket(m_ClientSocket);
	WSACleanup();
}




//void NetworkManager::WorkerThread( Session* session )
//{
//	char buffer[1024]{ 0 };
//	// send();
//	while (true)
//	{
//		printf_s( "입력 : " );
//		scanf_s( buffer );
//		int ret = send(session->clientSocket, buffer, strlen(buffer), 0);
//		
//		recv(session->clientSocket, buffer, ret, 0 );
//
//	}
//
//}
