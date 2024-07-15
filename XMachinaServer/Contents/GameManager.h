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
	std::vector<SPtr_GameRoom>  mRooms         = {};
	UINT32						mRoomMaxCnt    = {};

	Lock::SRWLock				mSRWLock       = {};

public:
	GameManager();
	~GameManager();

public:
	void Init();

public:
	bool EnterInRoom(SPtr_GamePlayer player, int roomid = -1); // -1 : ENTER IN ANY ROOM 
	bool ExitInRoom(SPtr_GamePlayer player);

	void BroadcastRoom(int roomid, SPtr_SendPktBuf& pkt, int exceptsessionid = -1 /* ������ ���� ��� */);
	void BroadcastAllRoom(SPtr_SendPktBuf& pkt);

	void Send(SPtr_SendPktBuf& pkt, int roomID, int sessionID); // Room �ȿ� �ش� Session�� ������ Send 


	std::vector<PlayerSnapShot> GetPlayerSnapShots_Room(int roomid);


};

