#pragma once
#define PORT 8888;

enum SC_PACKET
{
	SC_ID_PUT,
	SC_GAME_INIT_INFO,
	SC_GAME_START,
	SC_MOVE,
	SC_USE_ITEM,
	SC_DEAD,
	SC_GAME_END,
};


enum CS_PACKET
{
	CS_READY,
	CS_UNREADY,
	CS_INIT_COMPLETE,
	CS_MOVE,
	CS_GAME_DISCONNECT,
};


#pragma pack(push, 1

struct BUG_OBJ_POS
{
	unsigned short x, y;
};

struct ITEM_OBJ_POS
{
	unsigned char type;
	unsigned short x, y;
};

struct PLAYER_OBJ_POS
{
	unsigned short x, t;
};



struct SC_ID_PUT_PACKET
{
	unsigned short length;
	unsigned char type;
	unsigned char id;

	SC_ID_PUT_PACKET(unsigned char id)
		: length(5), type(SC_ID_PUT), id(id)
	{

	}
};
struct SC_GAME_INIT_INFO_PACKET
{
	unsigned short length;
	unsigned char type;
	unsigned char id1;
	PLAYER_OBJ_POS pos1;
	unsigned char id2;
	PLAYER_OBJ_POS pos2;
	BUG_OBJ_POS bug_pos[100];
	ITEM_OBJ_POS item_pos[5];

};
struct SC_GAME_START_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	SC_GAME_START_PACKET()
		: length(5), type(SC_GAME_START), sign(0xFFFF)
	{
	}
};


struct SC_MOVE_PACKET
{
	unsigned short length;
	unsigned char type;
	unsigned char id1;
	PLAYER_OBJ_POS pos1;
	unsigned char id2;
	PLAYER_OBJ_POS pos2;
	BUG_OBJ_POS bug_pos[100];
	ITEM_OBJ_POS item_pos[5];


};
struct SC_USE_ITEM_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned char	id1;
	unsigned char	item1;
	unsigned char	id2;
	unsigned char	item2;

	SC_USE_ITEM_PACKET()
		: length(7), type(SC_USE_ITEM), id1(0), item1(0), id2(0), item2(0)
	{
	}

};
struct SC_DEAD_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned char	id1;
	bool	is_dead1;
	unsigned char	id2;
	bool	is_dead2;
	SC_DEAD_PACKET()
		: length(7), type(SC_DEAD), id1(0), is_dead1(false), id2(0), is_dead2(false)
	{
	}
};
struct SC_GAME_END_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	SC_GAME_END_PACKET()
		: length(5), type(SC_GAME_END), sign(0x0000)
	{
	}
};


struct CS_READY_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	CS_READY_PACKET()
		: length(5), type(CS_READY), sign(0xFFFF)
	{
	}
};
struct CS_UNREADY_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	CS_UNREADY_PACKET()
		: length(5), type(CS_UNREADY), sign(0xFF00)
	{
	}
};
struct CS_INIT_COMPLETE_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	CS_INIT_COMPLETE_PACKET()
		: length(5), type(CS_INIT_COMPLETE), sign(0x00FF)
	{
	}

};
struct CS_MOVE_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned char	id;
	unsigned short	x;
	unsigned short	y;

	CS_MOVE_PACKET(unsigned short x = 0, unsigned short y = 0)
		: length(9), type(CS_MOVE), x(x), y(y)
	{
	}

};
struct CS_GAME_DISCONNECT_PACKET
{
	unsigned short	length;
	unsigned char	type;
	unsigned short	sign;
	CS_GAME_DISCONNECT_PACKET()
		: length(5), type(CS_GAME_DISCONNECT), sign(0x0000)
	{
	}
};

#pragma pack(pop)