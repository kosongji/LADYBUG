#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"

#include "ObjectManager.h"



CS::ObjectManager::ObjectManager(Sync * sync)
	: m_Players ( sync->players ),
	 m_Items ( sync->items ),
	 m_Bugs ( sync->bugs )
{
}

CS::ObjectManager::~ObjectManager()
{

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
