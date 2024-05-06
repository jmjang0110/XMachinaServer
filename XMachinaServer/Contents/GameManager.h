#pragma once

/// +-------------------------------
///		  Game Manager 
/// ________________________________
/// 
///	 manage Game World, Room, Scene
/// 
/// -------------------------------+

#include "GamePlayer.h"

#define GAME_MGR GameManager::GetInst()
class GameManager
{
	DECLARE_SINGLETON(GameManager);

private:
	Lock::SRWLock mSRWLock       = {};
	
	UINT32	mRoomMaxCnt               = {};
	std::vector<SPtr_GameRoom> mRooms = {};

public:
	GameManager();
	~GameManager();

public:
	void Init();

public:
	bool EnterInRoom(SPtr_GamePlayer player, int roomid = -1); // -1 : ENTER IN ANY ROOM 
	bool ExitInRoom(SPtr_GamePlayer player);

	void BroadcastRoom(int roomid, SPtr_SendPktBuf& pkt, int exceptsessionid = -1 /* 제외할 세션 대상 */);

	std::vector<PlayerInfo> GetPlayerInfos_Room(int roomid);


};

