#pragma once

/// +-------------------------------
///		  Game Manager 
/// ________________________________
/// 
///	 manage Game World, Room, Scene
/// 
/// -------------------------------+

#define ROOM_MGR RoomManager::GetInst()
class GameRoom;
class GameObject;
class RoomManager
{
	DECLARE_SINGLETON(RoomManager);

private:
	std::vector<SPtr<GameRoom>>  mRooms         = {};
	UINT32						mRoomMaxCnt    = {};

	Lock::SRWLock				mSRWLock       = {};

public:
	RoomManager();
	~RoomManager();

public:
	void Init();

public:
	bool EnterInRoom(SPtr<GameObject> player, int roomid = -1); // -1 : ENTER IN ANY ROOM 
	bool ExitInRoom(SPtr<GameObject> player);

	void BroadcastRoom(int roomid, SPtr_SendPktBuf& pkt, int exceptsessionid = -1 /* ������ ���� ��� */);
	void BroadcastAllRoom(SPtr_SendPktBuf& pkt);

	void Send(SPtr_SendPktBuf& pkt, int roomID, int sessionID); // Room �ȿ� �ش� Session�� ������ Send 

	std::vector<SPtr<GameObject>> GetAllPlayersInRoom(int roomID);
};

