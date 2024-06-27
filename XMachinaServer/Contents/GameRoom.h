#pragma once
#include "GamePlayer.h"
#include "PlayerController.h"
#include "NPCController.h"
#include "SectorController.h"

class GameObject;
namespace RoomInfo
{
	constexpr int MaxSessionSize = 10; /* �ϳ��� Room �� 10 ���� Session �� �޵��� �Ѵ�. */
};
class GameRoom : public GameEntity
{

private:
	int					mID = -1; /* ROOM ID */
	PlayerController*	mPC = {};
	NPCController*		mNC = {};
	SectorController*	mSC = {};


public:
	GameRoom();
	~GameRoom();

public:
	/// +-------------------------------------------------------------------
	///	������ Room
	/// -------------------------------------------------------------------+
	void SetRoomID(int id) { mID = id; }
	void Init(int roomid);
	bool IsPossibleToEnter(); 

public:
	/// +-------------------------------------------------------------------
	///	������ Player Controller 
	/// -------------------------------------------------------------------+
	bool EnterPlayer(SPtr_GamePlayer player);
	bool ExitPlayer(UINT32 id);
	void Broadcast(SPtr_SendPktBuf packet, UINT32 exceptsessionid = -1);
	void SendPacket(UINT32 sessionid, SPtr_SendPktBuf packet);
	std::vector<PlayerInfo> GetInsertedPlayersInfo();

public:
	/// +-------------------------------------------------------------------
	///	������ NPC Controller 
	/// -------------------------------------------------------------------+

public:
	/// +-------------------------------------------------------------------
	///	������ Sector Controller 
	/// -------------------------------------------------------------------+


	
};

