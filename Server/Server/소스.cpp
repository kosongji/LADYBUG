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

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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
		printf("���۷� : %.1f\n", percent);
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

	//���� ����
	char buf[BUFSIZE + 1]; //���ۻ�����
	int len; //���ϱ���
	char FileName[256];//�����̸�
	int portInfo;

	//Ŭ���̾�Ʈ ���� ���
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
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));*/

		// Ŭ���̾�Ʈ�� ������ ���. �������̷� ������ ũ�⸦ �޾ƿ�
		retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;


		//�����̸��� �޾ƿ�
		recvn(client_sock, FileName, 256, 0);
		FILE* file = fopen(FileName, "wb");

		for (int i = 0; i < portCount; ++i)
		{
			if (portNum[i] == ntohs(clientaddr.sin_port))
			{
				gotoxy(LINEWIDTH, LINEHEIGHT * i + 1);
			}
		}
		printf("[%s]\t���� ũ�� : %d \n", FileName, len);

		while (1) {

			//���ϳ����� �޾ƿͼ� ���Ͽ� �ۼ���
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			fwrite(buf, 1, retval, file);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ���� ������ ���
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
	printf("[%s]\t���ۿϷ�\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		FileName, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
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
		// ������ Ŭ���̾�Ʈ ���� ���
		//printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		//	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


		//������ ����
		hThread[portCount] = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) //�ڵ鰪�� ���ϰ��
			closesocket(client_sock);
		//else
		//	CloseHandle(hThread);




	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}