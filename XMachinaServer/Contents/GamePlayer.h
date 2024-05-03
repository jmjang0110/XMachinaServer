#pragma once
#undef max
#include "Enum_generated.h"
#include "GameObject.h"

/// +-------------------------------
///		     GamePlayer
/// ________________________________
///
/// GAME ROOM 
///		��
/// GAME PLAYER �� GAME SESSION ( Owner )
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

/* �÷��̾� ���� */
struct PlayerInfo
{
	SPtr_GameSession		Owner     = {}; /* OWNER SESSION */

	long long				Timestamp = {};

	UINT32					PlayerID  = -1; /* PLAYER ID */
	UINT32					RoomID	  = -1; /* ROOM NUMBER */
	std::string				Name      = {};
	FBProtocol::OBJECTTYPE	Type      = FBProtocol::OBJECTTYPE::OBJECTTYPE_PLAYER;


	Vec3					Position  = {};
	Vec3					Scale	  = Vec3(1.f, 1.f, 1.f);
	Vec3					Rotation  = {};

	Vec3					FrontDir  = {};
	Vec3					SpineDir  = {};

	PlayerInfo(){}
	PlayerInfo(UINT32 id, std::string name, FBProtocol::OBJECTTYPE type) { PlayerID = id, Name = name, Type = type; }
	~PlayerInfo() { Owner = nullptr; /* Decrease Ref */ };
};

/* GAME PLAYER */
class GamePlayer : public GameObject
{
private:
	Lock::SRWLockGuard mSRWLock;
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
	void SetType(FBProtocol::OBJECTTYPE type)	{ mInfo.Type     = type; };

	/* UPDATE FREQUENTLY --- DATA RACE ( Read / Write ) In MultiThreads */
	void SetPosition(Vec3 pos)					{ mSRWLock.LockWrite(); mInfo.Position = pos;  mSRWLock.UnlockWrite(); };
	void SetRotation(Vec3 Rot)					{ mSRWLock.LockWrite(); mInfo.Rotation = Rot;  mSRWLock.UnlockWrite(); };
	void SetFrontDir(Vec3 FDir)					{ mSRWLock.LockWrite(); mInfo.FrontDir = FDir; mSRWLock.UnlockWrite(); };
	void SetSpineDir(Vec3 SDir)					{ mSRWLock.LockWrite(); mInfo.SpineDir = SDir; mSRWLock.UnlockWrite(); };

	PlayerInfo GetInfo() { mSRWLock.LockRead(); PlayerInfo currInfo = mInfo; mSRWLock.UnlockRead(); return currInfo; };

public:
	void DecRef_OwnerGameSession() { mInfo.Owner = nullptr; }

};

