#pragma once
#undef max
#include "Enum_generated.h"
#include "GameObject.h"
#include "GameMonster.h"
#include "GameBullet.h"

#include "ObjectSnapShot.h"
#include "Skill.h"
#include "DB_Weapon.h"
#include "ViewList.h"


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



/* 플레이어 상태 */
struct PlayerSnapShot : public ObjectSnapShot
{	
	/// +-----------------------------------------------------------
	///		base 
	/// -----------------------------------------------------------+
	std::string				Name			= {};
	SPtr<GameSession>		Owner			= {};								// Owner Session 
	UINT32					RoomID			= -1;								// Owner Room ID 
	std::vector<Coordinate>	CurSectorID		= {};								// Current Secotr Indexes

	bool IsExit = false;	Lock::SRWLock Lock_IsExit;							/// > [LOCK] check Player Disconndected 

	/// +-----------------------------------------------------------
	///		View List 
	/// -----------------------------------------------------------+
	float					ViewRangeRadius = 40.f;								// View Range 
	ViewList				Vlist           = {};								// View List ( Current ) 
	ViewList				VList_Prev      = {};								// View List (  Prev  )

	/// +-----------------------------------------------------------
	///		Skill 
	/// -----------------------------------------------------------+
	bool					ActiveSkills[static_cast<UINT8>(SkillInfo::Type::End)]{ false, }; Lock::SRWLock Lock_ActiveSkills;

	float					HP;     Lock::SRWLock lock_HP;						/// > [LOCK] HP 
	float					Shield; Lock::SRWLock Lock_Shield;					/// > [LOCK] shield 
	/// +-----------------------------------------------------------
	///		Item 
	/// -----------------------------------------------------------+
	FBProtocol::WEAPON_TYPE	WeaponType = {};	Lock::SRWLock Lock_Weapon;		/// > [LOCK] Current Equipped Weapon 

	std::array<SPtr<GameBullet>, GameObjectInfo::maxBulletsNum> Bullets;		// Bullets 
	Concurrency::concurrent_queue<int> mPossibleBulletIndex;					// Possible To Shoot Bullets 

	/// +-----------------------------------------------------------
	///		latency 
	/// -----------------------------------------------------------+
	long long				Timestamp = {};										// Latency 

	/// +-----------------------------------------------------------
	///		Transform 
	/// -----------------------------------------------------------+
	float					Velocity = {};	Lock::SRWLock Lock_Velocity;		/// > [LOCK] Move Speed 
	Vec3					SpineLookDir = {}; Lock::SRWLock Lock_SpineLookDir; /// > [LOCK] Spine Look Direction 

	/// +-----------------------------------------------------------
	///		Phero 
	/// -----------------------------------------------------------+
	float					Phero;  Lock::SRWLock Lock_Phero; // 현재 페로 양 



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

	void Exit();
	int OnShoot(); // return bullet id 

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
	void SetSNS_Velocity(float vel)										{ mInfo.Lock_Velocity.LockWrite();		mInfo.Velocity = vel;				 mInfo.Lock_Velocity.UnlockWrite(); }
	void SetSNS_SpineLookDir(Vec3 spineLookDir)							{ mInfo.Lock_SpineLookDir.LockWrite();	mInfo.SpineLookDir = spineLookDir;   mInfo.Lock_SpineLookDir.UnlockWrite(); }
	void SetSNS_Phero(float phero)										{ mInfo.Lock_Phero.LockWrite();			mInfo.Phero    = phero;				 mInfo.Lock_Phero.UnlockWrite(); }
	void SetSNS_ActiveSkill(SkillInfo::Type skillType, bool isActive)	{ mInfo.Lock_ActiveSkills.LockWrite();	mInfo.ActiveSkills[static_cast<UINT8>(skillType)] = isActive; mInfo.Lock_ActiveSkills.UnlockWrite(); }
	void SetSNS_EquipWeapon(FBProtocol::WEAPON_TYPE weaponType)			{ mInfo.Lock_Weapon.LockWrite();		mInfo.WeaponType = weaponType;		 mInfo.Lock_Weapon.UnlockWrite(); return; }
	

public:
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	
	PlayerSnapShot			GetSnapShot()									{ mSRWLock_SnapShot.LockRead(); PlayerSnapShot currInfo = mInfo; mSRWLock_SnapShot.UnlockRead(); return currInfo; };
	SPtr<GameSession>		GetSessionOwner()								{ return mInfo.Owner; };
	PlayerController*		GetOwnerPlayerController()						{ return mOwnerPC; }
	float					GetViewRangeRadius()							{ return mInfo.ViewRangeRadius; }

	std::string				GetName()										{ return mInfo.Name; }
	UINT32					GetRoomID()										{ return mInfo.RoomID; }
	long long				GetTimestamp()									{ return mInfo.Timestamp; }

	float					GetSNS_Velocity()								{ mInfo.Lock_Velocity.LockRead();		float vel                    = mInfo.Velocity; mInfo.Lock_Velocity.UnlockRead(); return vel; }
	bool					GetSNS_ActiveSkill(SkillInfo::Type skillType)	{ mInfo.Lock_ActiveSkills.LockRead();	bool active                  = mInfo.ActiveSkills[static_cast<UINT8>(skillType)]; mInfo.Lock_ActiveSkills.UnlockRead(); return active; }
	float					GetSNS_Phero()									{ mInfo.Lock_Phero.LockRead();			float phero                  = mInfo.Phero; mInfo.Lock_Phero.UnlockRead(); return phero; }
	FBProtocol::WEAPON_TYPE	GetSNS_CurrWeapon()								{ mInfo.Lock_Weapon.LockRead();			FBProtocol::WEAPON_TYPE name = mInfo.WeaponType; ; mInfo.Lock_Weapon.UnlockRead(); return name; }
	Vec3					GetSNS_SpineLookDir()							{ mInfo.Lock_SpineLookDir.LockRead();	Vec3 spineLookDir            = mInfo.SpineLookDir; mInfo.Lock_SpineLookDir.UnlockRead(); return spineLookDir; }

	bool IsExit()															{ mInfo.Lock_IsExit.LockWrite(); bool isExit = mInfo.IsExit; mInfo.Lock_IsExit.UnlockWrite(); return isExit; };

	// Get all active skills
	std::vector<bool> GetSNS_ActiveSkills() {
		mInfo.Lock_ActiveSkills.LockRead();
		std::vector<bool> activeSkills(static_cast<UINT8>(SkillInfo::Type::End));
		for (size_t i = 0; i < static_cast<size_t>(SkillInfo::Type::End); ++i) { activeSkills[i] = mInfo.ActiveSkills[i]; }
		mInfo.Lock_ActiveSkills.UnlockRead();
		return activeSkills;
	}

public:
	void DecRef_OwnerGameSession() { mInfo.Owner = nullptr; }
	void UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> montser);
	
	void CollideCheckWithBullets();
	void CollideCheckWithMonsters();


public:
	static Coordinate GetSectorIdx(Vec3& Pos);

};

