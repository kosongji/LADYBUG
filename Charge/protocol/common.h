#pragma once



#include <iostream>
#include <thread>


#include <WinSock2.h>
#include <stdio.h>


#include "const.h"



#pragma comment(lib , "ws2_32")



int recvn(SOCKET s, char *buf, int len, int flags);

void gotoxy(int x, int y);


