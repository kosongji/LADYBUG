#pragma once
#include "const.h"
#define MY_PORT 8888

enum SC_PACKET
{
	SC_ID_PUT,
	SC_GAME_INIT_INFO,
	SC_GAME_START,
	SC_MOVE_OBJ,
	SC_USE_ITEM,
	SC_DEAD,
	SC_GAME_END
};

enum CS_PACKET
{
	CS_READY			=	0,
	CS_UNREADY			=	1,
	CS_INIT_COMPLETE	=	2,
	CS_MOVE_UP			=	4,
	CS_MOVE_DOWN		=	5,
	CS_MOVE_LEFT		=	6,
	CS_MOVE_RIGHT		=	7,
	CS_GAME_DISCONNECT	=	8
};


#pragma pack(push, 1)

struct Header 
{
	unsigned short	length;
	unsigned char	type;

	Header( unsigned short length, unsigned char type )
		:length(length), type(type)
	{
	}
};


// 예시 
struct SC_PLAYER_MOVE_PACKET 
	: Header
{
	float x, y;
	SC_PLAYER_MOVE_PACKET(unsigned short length,float x, float y)
		: Header(length, SC_MOVE_OBJ), x(x), y(y)
	{
	}
};


struct SC_ID_PUT_PACKET
	: Header
{
	unsigned char id;

	SC_ID_PUT_PACKET(unsigned char id)
		: Header(5,SC_ID_PUT), id(id)
	{
	}
};


struct SC_GAME_INIT_INFO_PACKET
	: Header
{
	unsigned char	id1;
	unsigned char	id2;

	PLAYER_OBJ_POS	pos[2];
	BUG_OBJ_POS		bug_pos[10];
	ITEM_OBJ_POS	item_pos[5];

	SC_GAME_INIT_INFO_PACKET(
		unsigned char	id1,
		unsigned char	id2,
		void* p1,
		void* p2,
		void* p3
	)
		:Header(
			5 + sizeof(PLAYER_OBJ_POS) * 2 
			+ sizeof(PLAYER_OBJ_POS) * 10
			+ sizeof(ITEM_OBJ_POS) * 5
			,SC_GAME_INIT_INFO)
		, id1(id1), id2(id2)
	{
		memmove(pos, p1, sizeof(PLAYER_OBJ_POS));
		memmove(bug_pos, p2, sizeof(BUG_OBJ_POS));
		memmove(item_pos, p3, sizeof(ITEM_OBJ_POS));
	}
};

struct SC_GAME_START_PACKET
	: Header
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	SC_GAME_START_PACKET()
		: Header(5, SC_GAME_START), sign(0xFFFF)
	{
	}
};

// 수정중
struct SC_MOVE_PACKET
	: Header
{
	unsigned char	id1;
	unsigned char	id2;

	PLAYER_OBJ_POS	pos[2];
	BUG_OBJ_POS		bug_pos[10];
	ITEM_OBJ_POS	item_pos[5];

	SC_MOVE_PACKET(
		unsigned char	id1,
		unsigned char	id2,
		void* p1,
		void* p2,
		void* p3
	)
		:Header(
			5 + sizeof(PLAYER_OBJ_POS) * 2
			+ sizeof(PLAYER_OBJ_POS) * 10
			+ sizeof(ITEM_OBJ_POS) * 5
			, SC_GAME_INIT_INFO)
		, id1(id1), id2(id2)
	{
		memmove(pos, p1, sizeof(PLAYER_OBJ_POS));
		memmove(bug_pos, p2, sizeof(BUG_OBJ_POS));
		memmove(item_pos, p3, sizeof(ITEM_OBJ_POS));
	}
};

struct SC_USE_ITEM_PACKET
	: Header
{
	unsigned char	id1;
	unsigned char	id2;
	unsigned char	item1;
	unsigned char	item2;

	SC_USE_ITEM_PACKET( 
		unsigned char	id1,
		unsigned char	item1,
		unsigned char	id2,
		unsigned char	item2
	)	
		: Header(7, SC_USE_ITEM), id1(id1), item1(item1)
		, id2(id2),item2(item2)
	{
	}

};

struct SC_DEAD_PACKET
	: Header
{
	unsigned char	id1;
	bool			is_dead1;
	unsigned char	id2;
	bool			is_dead2;
	SC_DEAD_PACKET()
		: Header(7, SC_DEAD), id1(0), is_dead1(false), id2(0), is_dead2(false)
	{
	}
};

struct SC_GAME_END_PACKET
	: Header
{
	unsigned short	sign;
	SC_GAME_END_PACKET()
		: Header(5, SC_GAME_END), sign(0x0000)
	{
	}
};


// Client to Server
struct CS_READY_PACKET
	: Header
{
	unsigned short	sign;
	CS_READY_PACKET()
		: Header(5, CS_READY), sign(0xFFFF)
	{
	}
};
struct CS_UNREADY_PACKET
	: Header
{
	unsigned short	sign;
	CS_UNREADY_PACKET()
		: Header (5, CS_UNREADY), sign(0xFF00)
	{
	}
};

struct CS_INIT_COMPLETE_PACKET
	: Header
{
	unsigned short	sign;
	CS_INIT_COMPLETE_PACKET()
		: Header(5, CS_INIT_COMPLETE), sign(0x00FF)
	{
	}

};

struct CS_MOVE_PACKET
	: Header
{
	unsigned char	id;

	CS_MOVE_PACKET(unsigned char type
	,unsigned char id)
		: Header(4, type), id(id)
	{
	}

};

struct CS_GAME_DISCONNECT_PACKET
	: Header
{
	unsigned short	sign;
	CS_GAME_DISCONNECT_PACKET()
		: Header(5, CS_GAME_DISCONNECT), sign(0x0000)
	{
	}
};

#pragma pack(pop)