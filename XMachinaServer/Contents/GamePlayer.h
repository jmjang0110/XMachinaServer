#pragma once
#undef max
#include "Enum_generated.h"
#include "GameObject.h"
#include "GameMonster.h"
#include "ObjectSnapShot.h"

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
	std::unordered_map<UINT32, SPtr<GamePlayer>> VL_Players;
	std::unordered_map<UINT32, SPtr<GameMonster>> VL_Monsters;

	bool TryInsertPlayer(UINT32 key, SPtr<GamePlayer> player)
	{
		auto monsterIt = VL_Players.find(key);
		// 이미 존재한다.
		if (monsterIt != VL_Players.end())
			return false;
		else
			VL_Players.insert(std::make_pair(key, player));
		
		return true;

	}
	bool TryInsertMonster(UINT32 key, SPtr<GameMonster> monster)
	{
		auto monsterIt = VL_Monsters.find(key);
		// 이미 존재한다.
		if (monsterIt != VL_Monsters.end())
			return false;
		else
		{
			VL_Monsters.insert(std::make_pair(key, monster));
			monster->Activate();
		}

		return true;
	}
	bool RemovePlayer(UINT32 key)
	{
		auto playerIt = VL_Players.find(key);
		if (playerIt != VL_Players.end())
		{
			VL_Players.erase(playerIt);
			return true;
		}
		return false;
	}

	bool RemoveMonster(UINT32 key)
	{
		auto monsterIt = VL_Monsters.find(key);
		if (monsterIt != VL_Monsters.end())
		{
			monsterIt->second->DeActivate();
			VL_Monsters.erase(monsterIt);
			return true;
		}
		return false;
	}

	void Clear()
	{
		for (auto& it : VL_Monsters)
			it.second->DecreaseRef();

		VL_Monsters.clear();
		VL_Players.clear();
	}

};

/* 플레이어 상태 */
struct PlayerSnapShot : public ObjectSnapShot
{	
	/// +-----------------------------------------------------------
	///		base 
	/// -----------------------------------------------------------+
	std::string				Name      = {};
	FBProtocol::OBJECT_TYPE	Type      = FBProtocol::OBJECT_TYPE::OBJECT_TYPE_PLAYER;
	SPtr_GameSession		Owner     = {}; /* OWNER SESSION */

	/// +-----------------------------------------------------------
	///		Info 
	/// -----------------------------------------------------------+
	UINT32					RoomID	  = -1; /* ROOM NUMBER */
	std::vector<Coordinate>	CurSectorID;	// 현재 속해있는 Sector index ( 여러개일 수 있음 ) 최대 4개..



	/// +-----------------------------------------------------------
	///		latency 
	/// -----------------------------------------------------------+
	long long				Timestamp = {};

	/// +-----------------------------------------------------------
	///		Transform 
	/// -----------------------------------------------------------+
	float					Velocity  = {};
	Vec3					Position  = {};
	Vec3					Rotation  = {};

	Vec3					FrontDir  = {};
	Vec3					SpineDir  = {};

	/// +-----------------------------------------------------------
	///		View List 
	/// -----------------------------------------------------------+
	float					ViewRangeRadius = 5.f;
	ViewList				Vlist;
	ViewList				VList_Prev;

	PlayerSnapShot(){}
	PlayerSnapShot(UINT32 id, std::string name, FBProtocol::OBJECT_TYPE type) { ObjectSnapShot::ID = id, Name = name, Type = type; }
	~PlayerSnapShot() { Owner = nullptr; /* Decrease Ref */ };
};




/* GAME PLAYER */
class GamePlayer : public GameObject
{
private:
	class PlayerController* mOwnerPC = nullptr;

	Lock::SRWLock mSRWLock;
	PlayerSnapShot	mInfo;

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
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetOWner(SPtr_GameSession gsession)			 { mInfo.Owner    = gsession; };
	void SetPlayerID(UINT32 playerid)					 { mInfo.ID       = playerid; };
	void setRoomID(UINT32 roomid)						 { mInfo.RoomID   = roomid; };
	void SetName(std::string name)						 { mInfo.Name     = name; };
	void SetType(FBProtocol::OBJECT_TYPE type)			 { mInfo.Type     = type; };
	/* UPDATE FREQUENTLY --- DATA RACE ( Read / Write ) In MultiThreads */
	void SetVelocity(float vel)							 { mSRWLock.LockWrite(); mInfo.Velocity = vel;			 mSRWLock.UnlockWrite(); }
	void SetPosition(Vec3 pos)							 { mSRWLock.LockWrite(); mInfo.Position = pos;			 mSRWLock.UnlockWrite(); }
	void SetRotation(Vec3 Rot)							 { mSRWLock.LockWrite(); mInfo.Rotation = Rot;			 mSRWLock.UnlockWrite(); }
	void SetFrontDir(Vec3 FDir)							 { mSRWLock.LockWrite(); mInfo.FrontDir = FDir;			 mSRWLock.UnlockWrite(); }
	void SetSpineDir(Vec3 SDir)							 { mSRWLock.LockWrite(); mInfo.SpineDir = SDir;			 mSRWLock.UnlockWrite(); }
	void SetSectorIdx(std::vector<Coordinate> sectorIdx) { mSRWLock.LockWrite(); mInfo.CurSectorID = sectorIdx;  mSRWLock.UnlockWrite(); }

	void SetSnapShot(PlayerSnapShot& info)				 { mSRWLock.LockWrite(); mInfo = info; mSRWLock.UnlockWrite(); }
	void SetOwnerPlayerController(PlayerController* pc)  { mOwnerPC = pc; }
	
public:
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	PlayerSnapShot		GetSnapShot()			        { mSRWLock.LockWrite(); PlayerSnapShot currInfo = mInfo; mSRWLock.UnlockWrite(); return currInfo; };
	SPtr<GameSession>	GetSessionOwner()		        { return mInfo.Owner; };
	PlayerController*	GetOwnerPlayerController()      { return mOwnerPC; }
	float				GetViewRangeRadius()	        { /* 혹시 ViewRange 를 바꾸는 일이 있다면 lock 을 걸어야한다. */ return mInfo.ViewRangeRadius; }
	ViewList			GetCurrViewList()		        { return mInfo.Vlist; }
	ViewList			GetPrevViewList()		        { return mInfo.VList_Prev; }

public:
	void DecRef_OwnerGameSession() { mInfo.Owner = nullptr; }
	void UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> montser);
	

public:
	static Coordinate GetSectorIdx(Vec3& Pos);

};

