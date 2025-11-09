#pragma once

enum EntityType : int
{
	ET_ALL			= -1,	//1111 1111
	ET_NONE			= 0,	//0000 0000
	ET_SHIP			= 1 << 0,	//0000 0001
	ET_BULLET_PLAYER = 1 << 1,	//0000 0010
	ET_BULLET_ENEMY = 1 << 2,	//0000 0100
	ET_ENEMY		= 1 << 3,	//0000 1000	
	ET_POWERUP		= 1 << 4,	//0001 0000
	ET_BOSS			= 1 << 5    //0010 0000
};

enum class State
{
	Start,
	RunGame,
	End
};

// int val = ET_ALL; Ok
// int val = State::Start; Error
// int val = (int)State::Start; Ok
// enum => ET_NONE
// enum class => state