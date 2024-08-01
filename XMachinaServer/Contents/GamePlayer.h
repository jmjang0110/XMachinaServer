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
///		��
/// GAME PLAYER �� GAME SESSION ( Owner )
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


/// +-----------------------------------------------------------
///					    PLAYER SNAPSHOT 
/// ____________________________________________________________
/// -----------------------------------------------------------+


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
	float					ViewRangeRadius = 10.f;								// View Range 
	ViewList				Vlist           = {};								// View List ( Current ) 
	ViewList				VList_Prev      = {};								// View List (  Prev  )

	/// +-----------------------------------------------------------
	///		Skill 
	/// -----------------------------------------------------------+
	std::array<SPtr<GameSkill>, FBProtocol::PLAYER_SKILL_TYPE_END> Skills = {};		// Bullets 

	float					HP     = {};     Lock::SRWLock lock_HP;						/// > [LOCK] HP 
	float					Shield = {};	 Lock::SRWLock Lock_Shield;					/// > [LOCK] shield 
	/// +-----------------------------------------------------------
	///		Item 
	/// -----------------------------------------------------------+
	FBProtocol::WEAPON_TYPE	WeaponType = {};	Lock::SRWLock Lock_Weapon;		/// > [LOCK] Current Equipped Weapon 

	std::array<SPtr<GameBullet>, GameObjectInfo::maxBulletsNum> Bullets = {};		// Bullets 
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
	float					Phero = {};  Lock::SRWLock Lock_Phero; // ���� ��� �� 



	PlayerSnapShot(){}
	PlayerSnapShot(UINT32 id, std::string name) { ObjectSnapShot::ID = id, Name = name; }
	~PlayerSnapShot() { Owner = nullptr; /* Decrease Ref */ };
};




/// +-----------------------------------------------------------
///					    GAME PLAYER 
/// ____________________________________________________________
/// -----------------------------------------------------------+
class GamePlayer : public GameObject
{
private:
	class PlayerController* mOwnerPC = nullptr;
	PlayerSnapShot	mSnapShot; Lock::SRWLock mSRWLock_SnapShot;

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

	void	Exit();
	bool	IsExit() { mSnapShot.Lock_IsExit.LockWrite(); bool isExit = mSnapShot.IsExit; mSnapShot.Lock_IsExit.UnlockWrite(); return isExit; };
	int		OnShoot();  

public:
	void DecRef_OwnerGameSession() { mSnapShot.Owner = nullptr; }
	void UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> montser);

	void CollideCheckWithBullets();
	void CollideCheckWithMonsters();

public:
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetOWner(SPtr_GameSession gsession)								{ mSnapShot.Owner    = gsession; };
	void SetPlayerID(UINT32 playerid)										{ mSnapShot.ID       = playerid; };
	void setRoomID(UINT32 roomid)											{ mSnapShot.RoomID   = roomid; };
	void SetName(std::string name)											{ mSnapShot.Name     = name; };
	void SetOwnerPlayerController(PlayerController* pc)						{ mOwnerPC = pc; }
	                   
	void SetSNS_Velocity(float vel)											{ mSnapShot.Lock_Velocity.LockWrite();		mSnapShot.Velocity = vel;				 mSnapShot.Lock_Velocity.UnlockWrite(); }
	void SetSNS_SpineLookDir(Vec3 spineLookDir)								{ mSnapShot.Lock_SpineLookDir.LockWrite();	mSnapShot.SpineLookDir = spineLookDir;   mSnapShot.Lock_SpineLookDir.UnlockWrite(); }
	void SetSNS_Phero(float phero)											{ mSnapShot.Lock_Phero.LockWrite();			mSnapShot.Phero    = phero;				 mSnapShot.Lock_Phero.UnlockWrite(); }
	void SetSNS_EquipWeapon(FBProtocol::WEAPON_TYPE weaponType)				{ mSnapShot.Lock_Weapon.LockWrite();		mSnapShot.WeaponType = weaponType;		 mSnapShot.Lock_Weapon.UnlockWrite(); return; }
	
public:
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	
	SPtr<GameSession>		GetSessionOwner()								{ return mSnapShot.Owner; };
	PlayerController*		GetOwnerPlayerController()						{ return mOwnerPC; }
	float					GetViewRangeRadius()							{ return mSnapShot.ViewRangeRadius; }

	std::string				GetName()										{ return mSnapShot.Name; }
	UINT32					GetRoomID()										{ return mSnapShot.RoomID; }
	long long				GetTimestamp()									{ return mSnapShot.Timestamp; }

	float					GetSNS_Velocity()								{ mSnapShot.Lock_Velocity.LockRead();		float vel                    = mSnapShot.Velocity; mSnapShot.Lock_Velocity.UnlockRead(); return vel; }
	float					GetSNS_Phero()									{ mSnapShot.Lock_Phero.LockRead();			float phero                  = mSnapShot.Phero; mSnapShot.Lock_Phero.UnlockRead(); return phero; }
	FBProtocol::WEAPON_TYPE	GetSNS_CurrWeapon()								{ mSnapShot.Lock_Weapon.LockRead();			FBProtocol::WEAPON_TYPE name = mSnapShot.WeaponType; ; mSnapShot.Lock_Weapon.UnlockRead(); return name; }
	Vec3					GetSNS_SpineLookDir()							{ mSnapShot.Lock_SpineLookDir.LockRead();	Vec3 spineLookDir            = mSnapShot.SpineLookDir; mSnapShot.Lock_SpineLookDir.UnlockRead(); return spineLookDir; }
	bool					GetSNS_SkillIsAvtive(FBProtocol::PLAYER_SKILL_TYPE type) { return mSnapShot.Skills[type]->GetIsActive(); }

public:
	static Coordinate GetSectorIdx(Vec3& Pos);

};

