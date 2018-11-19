#include "../protocol/common.h"
#include "../protocol/const.h"
#include "../protocol/protocol.h"

#include "ObjectManager.h"

namespace CS
{
	ObjectManager*  GObjectManager = nullptr;
}

 CS::ObjectManager::ObjectManager()
{
	m_Players = nullptr;
	m_Items = nullptr;
	m_Bugs = nullptr;
}

 CS::ObjectManager::ObjectManager(Sync * sync)
	: m_Players ( sync->players ),
	 m_Items ( sync->items ),
	 m_Bugs ( sync->bugs )
{
}

 CS::ObjectManager::~ObjectManager()
{

}

void  CS::ObjectManager::Initialize(Sync * sync)

{
	m_Players = sync->players;
	m_Items = sync->items;
	m_Bugs = sync->bugs;
}

PLAYER_OBJ_POS*  CS::ObjectManager::GetPlayers( )
{
	return m_Players;
}

ITEM_OBJ_POS*  CS::ObjectManager::Getitems( )
{
	return m_Items;
}

BUG_OBJ_POS*  CS::ObjectManager::GetBugs( )
{
	return m_Bugs;
}

void  CS::ObjectManager::SetPlayers(char * buf, int count = sizeof(PLAYER_OBJ_POS) * 2)
{
	memcpy(m_Players, buf, sizeof(PLAYER_OBJ_POS) * count);
}

void  CS::ObjectManager::SetItems(char * buf , int count)
{
	//memcpy(m_Items, buf, sizeof(ITEM_OBJ_POS) * count);

}

void  CS::ObjectManager::SetBugs(char * buf, int count)
{
	memcpy(m_Bugs, buf, sizeof(BUG_OBJ_POS) * count);

}
