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
		H_Look, 
		DBMS, 
		Stuart, 
		Descriptor, 
		T_12, 
		Pipeline, 
		BurnOut, 
		Direct_Drain
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

struct ViewList
{
	std::vector<SPtr<GamePlayer>> VL_Players;
	std::vector<SPtr<GameMonster>> VL_Monsters;

};

/* 플레이어 상태 */
struct PlayerSnapShot
{
	std::string				Name      = {};

	FBProtocol::OBJECT_TYPE	Type      = FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER;
	SPtr_GameSession		Owner     = {}; /* OWNER SESSION */


	UINT32					PlayerID  = -1; /* PLAYER ID */ /// - 여기서 ID는 현재 접속 후의 일시적인 아이디이다. DB에서 정보를 받을려면 stirngID를 이용해야한다.
	UINT32					RoomID	  = -1; /* ROOM NUMBER */
	std::vector<Coordinate>	CurSectorID;	// 현재 속해있는 Sector index ( 여러개일 수 있음 ) 최대 4개..

	long long				Timestamp = {};
	float					Velocity  = {};

	Vec3					Position  = {};
	Vec3					Rotation  = {};

	Vec3					FrontDir  = {};
	Vec3					SpineDir  = {};

	float					ViewRangeRadius = 5.f;
	ViewList				Vlist;

	PlayerSnapShot(){}
	PlayerSnapShot(UINT32 id, std::string name, FBProtocol::OBJECT_TYPE type) { PlayerID = id, Name = name, Type = type; }
	~PlayerSnapShot() { Owner = nullptr; /* Decrease Ref */ };
};




/* GAME PLAYER */
class GamePlayer : public GameObject
{
private:
	class PlayerController* mOwnerPC = nullptr;

	Lock::SRWLock mSRWLock;
	PlayerSnapShot	mInfo = {};

public:
	GamePlayer();
	GamePlayer(UINT32 sessionID, SPtr_GameSession owner);
	~GamePlayer();

public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

	 
public:
	/* UPDATE ONCE */
	void SetOWner(SPtr_GameSession gsession)	{ mInfo.Owner    = gsession; };
	void SetPlayerID(UINT32 playerid)			{ mInfo.PlayerID = playerid; };
	void setRoomID(UINT32 roomid)				{ mInfo.RoomID   = roomid; };
	void SetName(std::string name)				{ mInfo.Name     = name; };
	void SetType(FBProtocol::OBJECT_TYPE type)	{ mInfo.Type     = type; };

	/* UPDATE FREQUENTLY --- DATA RACE ( Read / Write ) In MultiThreads */
	void SetVelocity(float vel)							 { mSRWLock.LockWrite(); mInfo.Velocity = vel;			 mSRWLock.UnlockWrite(); }
	void SetPosition(Vec3 pos)							 { mSRWLock.LockWrite(); mInfo.Position = pos;			 mSRWLock.UnlockWrite(); }
	void SetRotation(Vec3 Rot)							 { mSRWLock.LockWrite(); mInfo.Rotation = Rot;			 mSRWLock.UnlockWrite(); }
	void SetFrontDir(Vec3 FDir)							 { mSRWLock.LockWrite(); mInfo.FrontDir = FDir;			 mSRWLock.UnlockWrite(); }
	void SetSpineDir(Vec3 SDir)							 { mSRWLock.LockWrite(); mInfo.SpineDir = SDir;			 mSRWLock.UnlockWrite(); }
	void SetSectorIdx(std::vector<Coordinate> sectorIdx) { mSRWLock.LockWrite(); mInfo.CurSectorID = sectorIdx;  mSRWLock.UnlockWrite(); }

	/* Get Set : Player Info */
	void SetSnapShot(PlayerSnapShot& info)	{ mSRWLock.LockWrite(); mInfo = info; mSRWLock.UnlockWrite(); }
	PlayerSnapShot GetSnapShot()			{ mSRWLock.LockRead(); PlayerSnapShot currInfo = mInfo; mSRWLock.UnlockRead(); return currInfo; };

	void SetOwnerPlayerController(PlayerController* pc) { mOwnerPC = pc; }
	PlayerController* GetOwnerPlayerController() { return mOwnerPC; }
	float GetViewRangeRadius() { /* 혹시 ViewRange 를 바꾸는 일이 있다면 lock 을 걸어야한다. */ return mInfo.ViewRangeRadius; }
public:
	void DecRef_OwnerGameSession() { mInfo.Owner = nullptr; }

};

