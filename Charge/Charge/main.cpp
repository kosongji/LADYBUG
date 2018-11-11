#include "common.h"
#include "protocol.h"
#include "NetworkManager.h"
#include "Packet/playerinfo_generated.h"

void main()
{

	NetworkManager networkMgr;
	networkMgr.Initialize();
		

	printf_s("Start Sertver");
	networkMgr.AcceptLoop();



	printf_s("End Server");
	networkMgr.Finalize();

}