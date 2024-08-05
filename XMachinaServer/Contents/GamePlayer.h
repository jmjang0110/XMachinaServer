#pragma once

#include "GameObject.h"
#include "GameMonster.h"
#include "GameBullet.h"

#include "ObjectSnapShot.h"
#include "GameSkill.h"
#include "DB_Weapon.h"
#include "ViewList.h"

#undef max
#include "Enum_generated.h"

/// +-------------------------------
///		     GamePlayer
/// ________________________________
///
/// GAME ROOM 
///		¡è
/// GAME PLAYER ¡æ GAME SESSION ( Owner )
/// 
/// 
/// SNS : Snap Shot 
/// -------------------------------+

/// +-----------------------------------------------------------
///					    PLAYER SNAPSHOT 
/// ____________________________________________________________
/// -----------------------------------------------------------+
struct PlayerSnapShot : public ObjectSnapShot
{	
	std::string				Name			= {};
	SPtr<GameSession>		Owner			= {};								// Owner Session 
	UINT32					RoomID			= -1;								// Owner Room ID 
	std::vector<Coordinate>	CurSectorID		= {};								// Current Secotr Indexes

	bool					IsExit			= false; Lock::SRWLock Lock_IsExit;				
	
	ViewList				VList_SnapShot  = {};	 Lock::SRWLock Lock_VList_SnapShot;		
	float					Phero           = {};	 Lock::SRWLock Lock_Phero;				
	FBProtocol::WEAPON_TYPE	WeaponType      = {};	 Lock::SRWLock Lock_Weapon;				
	float					Velocity        = {};	 Lock::SRWLock Lock_Velocity;			
	Vec3					SpineLookDir    = {};    Lock::SRWLock Lock_SpineLookDir;		


	PlayerSnapShot(){}
	PlayerSnapShot(UINT32 id, std::string name) { ObjectSnapShot::ID = id, Name = name; }
	~PlayerSnapShot() { Owner = nullptr; /* Decrease Ref */ };
};




/// +-----------------------------------------------------------
///					    GAME PLAYER 
/// ____________________________________________________________
/// -----------------------------------------------------------+
class Script_PlayerStat;
class GamePlayer : public GameObject
{
private:
	PlayerSnapShot				mSnapShot;	Lock::SRWLock mSRWLock_SnapShot;
	class PlayerController*		mOwnerPC	= nullptr;
	Script_PlayerStat*			mPlayerStat = {}; 

private:
	// Skills
	std::array<SPtr<GameSkill>, FBProtocol::PLAYER_SKILL_TYPE_END>		mSkills  = {};		// Bullets 
	
	// Bullets
	std::array<SPtr<GameBullet>, GameObjectInfo::maxBulletsNum>			mBullets = {};		// Bullets 
	Concurrency::concurrent_queue<int>									mPossibleBulletIndex;		// Possible To Shoot Bullets 
	
	// View List
	float					mViewRangeRadius = 10.f;										// View Range 
	ViewList				mVlist           = {};										// View List ( Current ) 

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
	int		OnShoot(Vec3& ray);
	bool	OnSkill(FBProtocol::PLAYER_SKILL_TYPE type);
	void	Exit();
	bool	IsExit() { mSnapShot.Lock_IsExit.LockWrite(); bool isExit = mSnapShot.IsExit; mSnapShot.Lock_IsExit.UnlockWrite(); return isExit; };
	


public:
	void DecRef_OwnerGameSession() { mSnapShot.Owner = nullptr; }
	void UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> montser);

	void CollideCheckWithBullets();
	void CollideCheckWithMonsters();

	void Push_PossibleBulletIndex(int idx);



public:
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetOWner(SPtr_GameSession gsession)								{ mSnapShot.Owner    = gsession; };
	void SetPlayerID(UINT32 playerid)										{ mSnapShot.ID       = playerid; };
	void setRoomID(UINT32 roomid)											{ mSnapShot.RoomID   = roomid; };
	void SetName(std::string name)											{ mSnapShot.Name     = name; };
	void SetOwnerPlayerController(PlayerController* pc)						{ mOwnerPC           = pc; }
	                   
	void SetSNS_Velocity(float vel)											{ mSnapShot.Lock_Velocity.LockWrite();		mSnapShot.Velocity     = vel;				 mSnapShot.Lock_Velocity.UnlockWrite(); }
	void SetSNS_SpineLookDir(Vec3 spineLookDir)								{ mSnapShot.Lock_SpineLookDir.LockWrite();	mSnapShot.SpineLookDir = spineLookDir;   mSnapShot.Lock_SpineLookDir.UnlockWrite(); }
	void SetSNS_Phero(float phero)											{ mSnapShot.Lock_Phero.LockWrite();			mSnapShot.Phero        = phero;				 mSnapShot.Lock_Phero.UnlockWrite(); }
	void SetSNS_EquipWeapon(FBProtocol::WEAPON_TYPE weaponType)				{ mSnapShot.Lock_Weapon.LockWrite();		mSnapShot.WeaponType   = weaponType;		 mSnapShot.Lock_Weapon.UnlockWrite(); return; }
public:
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	
	SPtr<GameSession>		GetSessionOwner()								{ return mSnapShot.Owner; };
	PlayerController*		GetOwnerPlayerController()						{ return mOwnerPC; }
	float					GetViewRangeRadius()							{ return mViewRangeRadius; }

	std::string				GetName()										{ return mSnapShot.Name; }
	UINT32					GetRoomID()										{ return mSnapShot.RoomID; }

	float					GetSNS_Velocity()								{ mSnapShot.Lock_Velocity.LockRead();		float vel                    = mSnapShot.Velocity; mSnapShot.Lock_Velocity.UnlockRead(); return vel; }
	float					GetSNS_Phero()									{ mSnapShot.Lock_Phero.LockRead();			float phero                  = mSnapShot.Phero; mSnapShot.Lock_Phero.UnlockRead(); return phero; }
	float					GetSNS_HP();
	FBProtocol::WEAPON_TYPE	GetSNS_CurrWeapon()								{ mSnapShot.Lock_Weapon.LockRead();			FBProtocol::WEAPON_TYPE name = mSnapShot.WeaponType; ; mSnapShot.Lock_Weapon.UnlockRead(); return name; }
	Vec3					GetSNS_SpineLookDir()							{ mSnapShot.Lock_SpineLookDir.LockRead();	Vec3 spineLookDir            = mSnapShot.SpineLookDir; mSnapShot.Lock_SpineLookDir.UnlockRead(); return spineLookDir; }
	GameSkill::State		GetSNS_SkillState(FBProtocol::PLAYER_SKILL_TYPE type)	{ return mSkills[type]->GetSNS_State(); }
	ViewList				GetSNS_ViewList()								{ mSnapShot.Lock_VList_SnapShot.LockRead(); ViewList vl = mSnapShot.VList_SnapShot; mSnapShot.Lock_VList_SnapShot.UnlockRead(); return vl; }
};

