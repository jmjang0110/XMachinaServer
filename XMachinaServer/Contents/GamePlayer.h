#pragma once
#undef max
#include "Enum_generated.h"
#include "GameObject.h"
#include "GameMonster.h"
#include "ObjectSnapShot.h"
#include "Skill.h"

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
	std::string				Name			= {};
	SPtr<GameSession>		Owner			= {};	/* OWNER SESSION */ 
	UINT32					RoomID			= -1;	/* ROOM NUMBER	*/
	std::vector<Coordinate>	CurSectorID;			 // 현재 속해있는 Sector index ( 여러개일 수 있음 ) 최대 4개..

	/// +-----------------------------------------------------------
	///		latency 
	/// -----------------------------------------------------------+
	long long				Timestamp		= {};

	/// +-----------------------------------------------------------
	///		Transform 
	/// -----------------------------------------------------------+
	float					Velocity = {}; Lock::SRWLock Lock_Velocity;
	Vec3					Position = {}; Lock::SRWLock Lock_Position;
	Vec3					Rotation = {}; Lock::SRWLock Lock_Rotation;

	Vec3					FrontDir = {}; Lock::SRWLock Lock_FrontDir;
	Vec3					SpineDir = {}; Lock::SRWLock Lock_SpineDir;

	/// +-----------------------------------------------------------
	///		Skill 
	/// -----------------------------------------------------------+
	bool					ActiveSkills[static_cast<UINT8>(SkillInfo::Type::End)]{ false, }; Lock::SRWLock Lock_ActiveSkills;

	float					HP;     Lock::SRWLock lock_HP;
	float					Shield; Lock::SRWLock Lock_Shield;
	/// +-----------------------------------------------------------
	///		Phero 
	/// -----------------------------------------------------------+
	float					Phero;  Lock::SRWLock Lock_Phero; // 현재 페로 양 

	/// +-----------------------------------------------------------
	///		View List 
	/// -----------------------------------------------------------+
	float					ViewRangeRadius = 5.f;
	ViewList				Vlist;
	ViewList				VList_Prev;

	PlayerSnapShot(){}
	PlayerSnapShot(UINT32 id, std::string name) { ObjectSnapShot::ID = id, Name = name; }
	~PlayerSnapShot() { Owner = nullptr; /* Decrease Ref */ };
};




/* GAME PLAYER */
class GamePlayer : public GameObject
{
private:
	class PlayerController* mOwnerPC = nullptr;

	Lock::SRWLock mSRWLock_SnapShot;
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
	void SetOwnerPlayerController(PlayerController* pc)  { mOwnerPC = pc; }

	/* UPDATE FREQUENTLY --- DATA RACE ( Read / Write ) In MultiThreads */
	void SetVelocity(float vel)							 { mInfo.Lock_Velocity.LockWrite();	mInfo.Velocity = vel;	mInfo.Lock_Velocity.UnlockWrite(); }
	void SetPosition(Vec3 pos)							 { mInfo.Lock_Position.LockWrite();	mInfo.Position = pos;	mInfo.Lock_Position.UnlockWrite(); }
	void SetRotation(Vec3 Rot)							 { mInfo.Lock_Rotation.LockWrite();	mInfo.Rotation = Rot;	mInfo.Lock_Rotation.UnlockWrite(); }
	void SetFrontDir(Vec3 FDir)							 { mInfo.Lock_FrontDir.LockWrite();	mInfo.FrontDir = FDir;	mInfo.Lock_FrontDir.UnlockWrite(); }
	void SetSpineDir(Vec3 SDir)							 { mInfo.Lock_SpineDir.LockWrite();	mInfo.SpineDir = SDir;	mInfo.Lock_SpineDir.UnlockWrite(); }
	void SetSnapShot(PlayerSnapShot& info)				 { mSRWLock_SnapShot.LockWrite();	mInfo          = info;	mSRWLock_SnapShot.UnlockWrite(); }
	void SetPhero(float phero)							 { mInfo.Lock_Phero.LockWrite();	mInfo.Phero    = phero; mInfo.Lock_Phero.UnlockWrite(); }
	void SetActiveSkill(SkillInfo::Type skillType, bool isActive) { mInfo.Lock_ActiveSkills.LockWrite(); mInfo.ActiveSkills[static_cast<UINT8>(skillType)] = isActive; mInfo.Lock_ActiveSkills.UnlockWrite(); }

public:
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	
	PlayerSnapShot		GetSnapShot()			        { mSRWLock_SnapShot.LockWrite(); PlayerSnapShot currInfo = mInfo; mSRWLock_SnapShot.UnlockWrite(); return currInfo; };
	SPtr<GameSession>	GetSessionOwner()		        { return mInfo.Owner; };
	PlayerController*	GetOwnerPlayerController()      { return mOwnerPC; }
	float				GetViewRangeRadius()	        { return mInfo.ViewRangeRadius; }

	// Getters
	std::string			GetName()		 { return mInfo.Name; }
	UINT32				GetRoomID()		 { return mInfo.RoomID; }
	long long			GetTimestamp()	 { return mInfo.Timestamp; }

	float				GetVelocity()	 { mInfo.Lock_Velocity.LockRead(); float vel = mInfo.Velocity; mInfo.Lock_Velocity.UnlockRead(); return vel; }
	Vec3				GetPosition()	 { mInfo.Lock_Position.LockRead(); Vec3 pos = mInfo.Position;  mInfo.Lock_Position.UnlockRead(); return pos; }
	Vec3				GetRotation()	 { mInfo.Lock_Rotation.LockRead(); Vec3 rot = mInfo.Rotation;  mInfo.Lock_Rotation.UnlockRead(); return rot; }
	Vec3				GetFrontDir()	 { mInfo.Lock_FrontDir.LockRead(); Vec3 front = mInfo.FrontDir; mInfo.Lock_FrontDir.UnlockRead(); return front; }
	Vec3				GetSpineDir()	 { mInfo.Lock_SpineDir.LockRead(); Vec3 spine = mInfo.SpineDir; mInfo.Lock_SpineDir.UnlockRead(); return spine; }
	bool				GetActiveSkill(SkillInfo::Type skillType)  { mInfo.Lock_ActiveSkills.LockRead(); bool active = mInfo.ActiveSkills[static_cast<UINT8>(skillType)]; mInfo.Lock_ActiveSkills.UnlockRead(); return active; }
	float				GetPhero()		 { mInfo.Lock_Phero.LockRead(); float phero = mInfo.Phero; mInfo.Lock_Phero.UnlockRead(); return phero; }
	// Get all active skills
	std::vector<bool> GetActiveSkills() {
		mInfo.Lock_ActiveSkills.LockRead();
		std::vector<bool> activeSkills(static_cast<UINT8>(SkillInfo::Type::End));
		for (size_t i = 0; i < static_cast<size_t>(SkillInfo::Type::End); ++i) { activeSkills[i] = mInfo.ActiveSkills[i]; }
		mInfo.Lock_ActiveSkills.UnlockRead();
		return activeSkills;
	}




public:
	void DecRef_OwnerGameSession() { mInfo.Owner = nullptr; }
	void UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> montser);
	

public:
	static Coordinate GetSectorIdx(Vec3& Pos);

};

