#pragma once
#undef max
#include "Enum_generated.h"
#include "GameObject.h"
#include "GameMonster.h"

/// +-------------------------------
///		     GamePlayer
/// ________________________________
///
/// GAME ROOM 
///		↑
/// GAME PLAYER → GAME SESSION ( Owner )
/// 
/// -------------------------------+
/// 
namespace PlayerEnum
{
	enum class WeaponType : UINT8 {
		H_Look, DBMS, Stuart, Descriptor, T_12, Pipeline, BurnOut, Direct_Drain
	};

	enum class MotionState : DWORD {
		None   = 0x00,
		Stand  = 0x01,
		Sit    = 0x02,
		Walk   = 0x10,
		Run    = 0x20,
		Sprint = 0x40
	};
}

/* 플레이어 상태 */
struct PlayerInfo
{
	FBProtocol::OBJECT_TYPE	Type      = FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER;
	SPtr_GameSession		Owner     = {}; /* OWNER SESSION */

	long long				Timestamp = {};

	UINT32					PlayerID  = -1; /* PLAYER ID */ /// - 여기서 ID는 현재 접속 후의 일시적인 아이디이다. DB에서 정보를 받을려면 stirngID를 이용해야한다.
	UINT32					RoomID	  = -1; /* ROOM NUMBER */
	std::vector<Coordinate>	CurSectorID;	// 현재 속해있는 Sector index ( 여러개일 수 있음 ) 최대 4개..
	std::string				Name      = {};


	float					Velocity  = {};

	Vec3					Position  = {};
	Vec3					Rotation  = {};

	Vec3					FrontDir  = {};
	Vec3					SpineDir  = {};

	PlayerInfo(){}
	PlayerInfo(UINT32 id, std::string name, FBProtocol::OBJECT_TYPE type) { PlayerID = id, Name = name, Type = type; }
	~PlayerInfo() { Owner = nullptr; /* Decrease Ref */ };
};

struct ViewList
{
	std::unordered_map<int, GameObject*> VL_Players;
	std::unordered_map<int, GameObject*> VL_Monsters;

};


/* GAME PLAYER */
class GamePlayer : public GameObject
{
private:
	Lock::SRWLock mSRWLock;
	PlayerInfo	mInfo = {};

public:
	GamePlayer();
	GamePlayer(UINT32 sessionID, SPtr_GameSession owner);
	~GamePlayer();
	 
public:
	/* UPDATE ONCE */
	void SetOWner(SPtr_GameSession gsession)	{ mInfo.Owner    = gsession; };
	void SetPlayerID(UINT32 playerid)			{ mInfo.PlayerID = playerid; };
	void setRoomID(UINT32 roomid)				{ mInfo.RoomID   = roomid; };
	void SetName(std::string name)				{ mInfo.Name     = name; };
	void SetType(FBProtocol::OBJECT_TYPE type)	{ mInfo.Type     = type; };

	/* UPDATE FREQUENTLY --- DATA RACE ( Read / Write ) In MultiThreads */
	void SetVelocity(float vel)					{ mSRWLock.LockWrite(); mInfo.Velocity = vel;  mSRWLock.UnlockWrite(); };
	void SetPosition(Vec3 pos)					{ mSRWLock.LockWrite(); mInfo.Position = pos;  mSRWLock.UnlockWrite(); };
	void SetRotation(Vec3 Rot)					{ mSRWLock.LockWrite(); mInfo.Rotation = Rot;  mSRWLock.UnlockWrite(); };
	void SetFrontDir(Vec3 FDir)					{ mSRWLock.LockWrite(); mInfo.FrontDir = FDir; mSRWLock.UnlockWrite(); };
	void SetSpineDir(Vec3 SDir)					{ mSRWLock.LockWrite(); mInfo.SpineDir = SDir; mSRWLock.UnlockWrite(); };

	void SetInfo(PlayerInfo& info)	{ mSRWLock.LockWrite(); mInfo = info; mSRWLock.UnlockWrite(); }
	PlayerInfo GetInfo()			{ mSRWLock.LockRead(); PlayerInfo currInfo = mInfo; mSRWLock.UnlockRead(); return currInfo; };

public:
	void DecRef_OwnerGameSession() { mInfo.Owner = nullptr; }

};

