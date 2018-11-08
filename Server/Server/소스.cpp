#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#define SERVERPORT 9000
#define BUFSIZE    512
#define PORTMAXNUM 3
#define LINEHEIGHT 10
#define LINEWIDTH 0
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;

		system("cls");

		float percent = ((float)left / (float)len) * 100;
		printf("전송률 : %.1f\n", percent);
	}

	return (len - left);
}

int portNum[PORTMAXNUM];
int portCount = 0;

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	//파일 정보
	char buf[BUFSIZE + 1]; //버퍼사이즈
	int len; //파일길이
	char FileName[256];//파일이름
	int portInfo;

	//클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1)
	{
		/*for (int i = 0; i < portCount; ++i)
		{
		if (portNum[i] == ntohs(clientaddr.sin_port))
		{
		gotoxy(LINEWIDTH, LINEHEIGHT * i);
		}
		}
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));*/

		// 클라이언트와 데이터 통신. 고정길이로 데이터 크기를 받아옴
		retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;


		//파일이름을 받아옴
		recvn(client_sock, FileName, 256, 0);
		FILE* file = fopen(FileName, "wb");

		for (int i = 0; i < portCount; ++i)
		{
			if (portNum[i] == ntohs(clientaddr.sin_port))
			{
				gotoxy(LINEWIDTH, LINEHEIGHT * i + 1);
			}
		}
		printf("[%s]\t파일 크기 : %d \n", FileName, len);

		while (1) {

			//파일내용을 받아와서 파일에 작성함
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			fwrite(buf, 1, retval, file);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 받은 데이터 출력
			//buf[retval] = '\0';
			//printf("buf : %s ", buf);


		}

		fclose(file);

	}

	// closesocket()
	closesocket(client_sock);

	for (int i = 0; i < portCount; ++i)
	{
		if (portNum[i] == ntohs(clientaddr.sin_port))
		{
			gotoxy(LINEWIDTH, LINEHEIGHT * i + 2);
		}
	}
	printf("[%s]\t전송완료\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		FileName, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	HANDLE hThread[3];


	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		portNum[portCount] = ntohs(clientaddr.sin_port);
		portCount++;

		/*for (int i = 0; i < portCount; ++i)
		{
		if (portNum[i] == ntohs(clientaddr.sin_port))
		{
		gotoxy(LINEWIDTH, LINEHEIGHT * i);
		}
		}*/
		// 접속한 클라이언트 정보 출력
		//printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		//	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


		//스레드 생성
		hThread[portCount] = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) //핸들값이 널일경우
			closesocket(client_sock);
		//else
		//	CloseHandle(hThread);




	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}