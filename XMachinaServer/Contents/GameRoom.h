#pragma once
#include "GamePlayer.h"

class GameObject;
enum class RoomInfo
{
	MaxSessionSize = 10, /* 하나의 Room 에 10 명의 Session 을 받도록 한다. */
};
class GameRoom
{
	//USE_LOCK;
	Lock::SRWLockGuard mSRWLock;


private:
	std::atomic_int32_t mCurrPlayerCnt = {};
	concurrency::concurrent_unordered_map<UINT32, SPtr_GamePlayer> mGamePlayers{};

public:
	/* GamePlayer */
	bool			InsertPlayer(SPtr_GameSession owner);
	bool			RemovePlayer(UINT32 sessionID);
	SPtr_GamePlayer FindPlayer(UINT32 sessionID);
	UINT32			GetPlayersSize() { return mCurrPlayerCnt.load(); }

	void Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID = -1); /* Broadcast Server Packet To Sessions In This Room */
	void SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt);

	std::vector<PlayerInfo> GetInsertedPlayersInfo();

};

