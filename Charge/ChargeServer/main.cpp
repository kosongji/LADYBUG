#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"

#include "NetworkManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"



void main()
{
	
	CS::GNetworkManager = new CS::NetworkManager();
	//CS::GObjectManager = new CS::ObjectManager();
	//CS::GNetworkManager = new CS::NetworkManager();
	CS::GNetworkManager->Initialize();
	
	//CS::GObjectManager->~ObjectManager();

	//CS::GObjectManager = new
	printf_s("Start Sertver");
	CS::GNetworkManager->AcceptLoop();


	printf_s("End Server");
	CS::GNetworkManager->Finalize();
		
}