#pragma once
#undef max
#include "Enum_generated.h"

struct PlayerInfo
{
	UINT32					PlayerID  = -1;
	std::string				Name      = {};
	FBProtocol::OBJECTTYPE	Type      = {};
	SPtr_GameSession		Owner     = {};

	PlayerInfo(){}
	PlayerInfo(UINT32 id, std::string name, FBProtocol::OBJECTTYPE type) { PlayerID = id, Name = name, Type = type; }
	~PlayerInfo() { Owner = nullptr; /* Decrease Ref */ };
};

class GamePlayer
{

};

