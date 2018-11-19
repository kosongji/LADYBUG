#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"


#include "ObjectManager.h"
#include "NetworkManager.h"


void main()
{

	CS::GNetworkManager = new CS::NetworkManager();
	CS::GObjectManager = new CS::ObjectManager();

	CS::GNetworkManager->Initialize();


	printf_s("Start Sertver");
	CS::GNetworkManager->CreateThread();


	printf_s("End Server");
	CS::GNetworkManager->Finalize();

	delete  CS::GNetworkManager;
	delete  CS::GObjectManager;

}