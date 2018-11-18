#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"
#include "../protocol/Singleton.h"

#include "NetworkManager.h"

void main()
{

	CS::NetworkManager* netMgr = new CS::NetworkManager;

	netMgr->Initialize();

	printf_s("Start Sertver %p \n ", netMgr);

	//printf_s("Start Sertver");
	netMgr->CreateThread();

	printf_s("End Server");
	netMgr->Finalize();

}