#pragma once
#include "GamePlayer.h"

class PlayerController;
class NPCController;
class SectorController;

class GameObject;
namespace RoomInfo
{
	constexpr int MaxSessionSize = 10; /* 하나의 Room 에 10 명의 Session 을 받도록 한다. */
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
	void PQCS(OverlappedObject* over);

public:
	/// +-------------------------------------------------------------------
	///	▶▶▶ Room
	/// -------------------------------------------------------------------+
	void SetRoomID(int id) {  mID = id; }
	void Init(int roomid);
	bool IsPossibleToEnter(); 

public:
	/// +-------------------------------------------------------------------
	///	▶▶▶ Player Controller 
	/// -------------------------------------------------------------------+
	bool EnterPlayer(SPtr_GamePlayer player);
	bool ExitPlayer(UINT32 id);
	void Broadcast(SPtr_SendPktBuf packet, UINT32 exceptsessionid = -1);
	void SendPacket(UINT32 sessionid, SPtr_SendPktBuf packet);
	std::vector<SPtr<GamePlayer>> GetallPlayers();

public:
	/// +-------------------------------------------------------------------
	///	▶▶▶ NPC Controller 
	/// -------------------------------------------------------------------+

public:
	/// +-------------------------------------------------------------------
	///	▶▶▶ Sector Controller 
	/// -------------------------------------------------------------------+


public:
	/// +-------------------------------------------------------------------
	///	▼ Getter 
	/// -------------------------------------------------------------------+
	PlayerController*	 GetPlayerController()		{ return mPC; }
	NPCController*		 GetNPCController()			{ return mNC; }
	SectorController*	 GetSectorController()		{ return mSC; }

};

