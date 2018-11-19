#pragma once


namespace CS
{

	class ObjectManager
	{
	public:
		ObjectManager();
		~ObjectManager();
		
		// ��Ŷ�� ������ �� ��������.
		inline PLAYER_OBJ_POS*	GetPlayers( );
		inline ITEM_OBJ_POS*	Getitems( );
		inline BUG_OBJ_POS*		GetBugs( );

		inline void SetPlayers(char*);
		inline void SetItems(char*);
		inline void SetBugs(char*);

	private: 
		PLAYER_OBJ_POS*		m_Players;
		ITEM_OBJ_POS*		m_Items;
		BUG_OBJ_POS*		m_Bugs;
	};


	extern ObjectManager* GObjectManager;
}


