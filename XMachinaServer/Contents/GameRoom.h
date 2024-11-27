#pragma once
#include "GameEntity.h"
class PlayerController;
class NPCController;
class SectorController;

class GameObject;

enum class RoomState
{
	Lobby,
	Battle,
};
namespace RoomInfo
{
	constexpr int MaxSessionSize = 500; /* �ϳ��� Room �� 10 ���� Session �� �޵��� �Ѵ�. */
};
class GameRoom : public GameEntity
{
private:
	int					mID = -1; /* ROOM ID */

	PlayerController*	mPC = {};
	NPCController*		mNC = {};
	SectorController*	mSC = {};

	RoomState mRoomState = RoomState::Lobby; Lock::SRWLock mLock_Roomstate;

public:
	GameRoom();
	~GameRoom();

public:
	void PQCS(OverlappedObject* over);
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);

public:
	/// +-------------------------------------------------------------------
	///	������ Room
	/// -------------------------------------------------------------------+
	void SetRoomID(int id) {  mID = id; }
	void Init(int roomid);
	bool IsPossibleToEnter(); 

public:
	/// +-------------------------------------------------------------------
	///	������ Player Controller 
	/// -------------------------------------------------------------------+
	bool EnterPlayer(SPtr<GameObject> player);
	bool ExitPlayer(UINT32 id);
	void Broadcast(SPtr_SendPktBuf packet, UINT32 exceptsessionid = -1);
	void SendPacket(UINT32 sessionid, SPtr_SendPktBuf packet);
	std::vector<SPtr<GameObject>> GetallPlayers();

public:
	/// +-------------------------------------------------------------------
	///	������ NPC Controller 
	/// -------------------------------------------------------------------+
	bool CollideCheckWithNPC(SPtr<GameObject> obj, ObjectTag objTag, int CheckSectorRadius);
	bool CollideCheckWithPlayer(SPtr<GameObject> obj);
	std::vector<SPtr<GameObject>> GetPlayersInRange(Vec3 ceneter_pos, float range_radius);

public:
	/// +-------------------------------------------------------------------
	///	������ Sector Controller 
	/// -------------------------------------------------------------------+


public:
	/// +-------------------------------------------------------------------
	///	�� Getter 
	/// -------------------------------------------------------------------+
	PlayerController*	 GetPlayerController()		{ return mPC; }
	NPCController*		 GetNPCController()			{ return mNC; }
	SectorController*	 GetSectorController()		{ return mSC; }

	void SetRoomState(RoomState roomstate) { mLock_Roomstate.LockWrite(); mRoomState = roomstate; mLock_Roomstate.UnlockWrite(); return; }
	RoomState GetRoomState() { mLock_Roomstate.LockRead(); RoomState roomstate = mRoomState; mLock_Roomstate.UnlockRead(); return roomstate; }
};

