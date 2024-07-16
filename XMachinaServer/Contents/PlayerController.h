#pragma once

#include "GamePlayer.h"

/// +-------------------------------
///		   PlayerController
/// ________________________________
///	> [ ���� ] 
/// - Room �ȿ� �ִ� Player ���� �����Ѵ�. 
/// 
/// 
/// -------------------------------+

class PlayerController
{

private:
	SPtr_GameRoom mOwnerRoom;

private:
	Lock::SRWLock mSRWLock;

	int mRoomID = -1;
	concurrency::concurrent_unordered_map<UINT32, SPtr_GamePlayer>	mGamePlayers;			// Key : ID / Value : Player ( Shared Ptr )
	std::atomic_int32_t												mCurrPlayerCnt = {};	/* CURRENT PLAYER COUNT IN THIS ROOM */

public:
	/* GamePlayer */
	void			Init(int roomID, SPtr_GameRoom owner);
	// WRITE Lock
	bool			EnterPlayer(SPtr_GamePlayer player); 
	// WRITE Lock
	bool			ExitPlayer(UINT32 sessionID);		 
	// READ  Lock 
	SPtr_GamePlayer FindPlayer(UINT32 sessionID);		
	
	void Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID = -1); /* Broadcast Server Packet To Sessions In This Room */
	void SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt);


	std::vector<PlayerSnapShot> GetInsertedPlayersInfo(); // READ Lock 
	std::vector<SPtr<GamePlayer>> GetPlayersInViewRange(Vec3 player_pos, float viewrange_radius);



	UINT32			GetPlayersSize() { return mCurrPlayerCnt.load(); }
	SPtr<GameRoom>  GetOwnerRoom() { return mOwnerRoom; }

public:
	PlayerController();
	~PlayerController();

};

