#include "common.h"
#include "protocol.h"
#include "NetworkManager.h"

void main()
{

	NetworkManager networkMgr;
	networkMgr.Initialize();
		

	printf_s("Start Sertver");


	printf_s("End Server");
	networkMgr.Finalize();

}