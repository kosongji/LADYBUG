#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"
#include "NetworkManager.h"
#include "SceneManager.h"

void main()
{


	NetworkManager networkMgr;
	networkMgr.Initialize();
		

	printf_s("Start Sertver");
	networkMgr.AcceptLoop();


	printf_s("End Server");
	networkMgr.Finalize();


}