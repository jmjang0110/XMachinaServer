#pragma once
/// +-------------------------------
///	       Player Data Base 
/// ________________________________
/// Manage Player Data In Game World 
/// -------------------------------+

#include "GamePlayer.h"

struct PlayerDataBase
{
	std::string stringID;

	std::string name;

	Vec3 Position;

	float HP;
	float MP;

};




class DB_Player
{
private:
	concurrency::concurrent_unordered_map<UINT32, PlayerDataBase> mConnectedPlayers; /* Players Database Infos In Current Game Status */

public:
	DB_Player();
	~DB_Player();

public:
	

};

