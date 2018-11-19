#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"

#include "NetworkManager.h"
#include "ObjectManager.h"

namespace CS
{
	ObjectManager* GObjectManager = nullptr;
}

CS::ObjectManager::ObjectManager()
{
	m_Players	= new PLAYER_OBJ_POS[PLAYER_NUM];
	m_Items		= new ITEM_OBJ_POS[BUG_NUM];
	m_Bugs		= new BUG_OBJ_POS[ITEM_NUM];
}

CS::ObjectManager::~ObjectManager()
{
	delete[] m_Players;
	delete[] m_Items;
	delete[] m_Bugs;
}

PLAYER_OBJ_POS * CS::ObjectManager::GetPlayers( )
{
	return m_Players;
}

ITEM_OBJ_POS * CS::ObjectManager::Getitems( )
{
	return m_Items;
}

BUG_OBJ_POS * CS::ObjectManager::GetBugs( )
{
	return m_Bugs;
}

void CS::ObjectManager::SetPlayers(char *)
{

}

void CS::ObjectManager::SetItems(char *)
{

}

void CS::ObjectManager::SetBugs(char *)
{

}
