#pragma once
#include "GamePlayer.h"

class GameObject;
namespace RoomInfo
{
	constexpr int MaxSessionSize = 10; /* �ϳ��� Room �� 10 ���� Session �� �޵��� �Ѵ�. */
};
class GameRoom : public GameEntity
{
	//USE_LOCK;
	Lock::SRWLockGuard mSRWLock;

private:
	int		mID = -1; /* ROOM ID */
	std::atomic_int32_t mCurrPlayerCnt = {}; /* CURRENT PLAYER COUNT IN THIS ROOM */
	concurrency::concurrent_unordered_map<UINT32, SPtr_GamePlayer> mGamePlayers;// Key : ID / Value : Player ( Shared Ptr )

public:
	GameRoom();
	~GameRoom();

public:
	/* GamePlayer */
	bool			EnterPlayer(SPtr_GamePlayer player); // WRITE Lock
	bool			ExitPlayer(UINT32 sessionID);		 // WRITE Lock
	SPtr_GamePlayer FindPlayer(UINT32 sessionID);		 // READ  Lock 

	UINT32			GetPlayersSize() { return mCurrPlayerCnt.load(); }

	void Broadcast(SPtr_SendPktBuf spkt, UINT32 exceptSessionID = -1); /* Broadcast Server Packet To Sessions In This Room */
	void SendPacket(UINT32 sessionID, SPtr_SendPktBuf sendPkt);

	std::vector<PlayerInfo> GetInsertedPlayersInfo(); // READ Lock 
	void SetRoomID(int id) { mID = id; }


	bool CheckIn(); /* CHECK ( IS IT POSSIBLE TO ENTER IN THIS ROOM? ) */

};

