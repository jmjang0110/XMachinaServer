#pragma once

#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "ObjectSnapShot.h"
#include "DB_Weapon.h"


/// +-------------------------------
///		     GameBullet
/// ________________________________
/// ▼
///	ID : Bullet Index in GamePlayer Bullets
/// -------------------------------+
class GameBullet;
class GamePlayer;
struct  BulletSnapShot : public ObjectSnapShot
{
	SPtr<GamePlayer>		PlayerOwner       = nullptr;
	SPtr<GameBullet>		BulletOwner       = nullptr;

	FBProtocol::ITEM_TYPE	WeaponType        = FBProtocol::ITEM_TYPE_NONE;	// 어떤 무기의 총알인가? 
	bool					IsActive          = false;	Lock::SRWLock Lock_IsActive;
};

class GameBullet : public GameObject	
{

private:
	SPtr<GamePlayer> mOwnerPlayer = nullptr;

private:
	BulletSnapShot  mInfo;
	std::atomic_int mActivate_Ref = 0;

	float			mMaxLifeTime  = 2.f;
	float			mCurrLifeTime = 0.f;
	Vec3			mOnShootDir   = {};
	int32_t			mHitMonsterID = {};

	float			mMaxMoveDist  = 40.f;
	float		    mCurrMoveDist = 0.f;

	float			mSpeed		  = 20.f;


public:
	GameBullet();
	GameBullet(UINT32 sessionID);
	GameBullet(UINT32 sessionID, SPtr<GamePlayer> owner);
	~GameBullet();
public:
	virtual void Update() override;

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

private:
	void BulletUpdate_Air_Strike();


public:
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }
	void BulletUpdate(FBProtocol::ITEM_TYPE weaponType);
	void CheckCollision_WithPlayerViewList();
	void CheckCollision_WithHitMonsterID_Ray();

	
public:
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetWeaponType(FBProtocol::ITEM_TYPE type)		{ mInfo.WeaponType = type;  }
	void SetOnShootDir(Vec3& dir)							{ mOnShootDir       = dir; }
	void SetHitMonsterID(int32_t id)						{ mHitMonsterID     = id; }
	void SetSpeed(float speed)								{ mSpeed            = speed; }


	// Getters
	int						GetActivate_RefCnt() { return mActivate_Ref.load(); }
    SPtr<GamePlayer>		GetOwnerPlayer()	 { return mOwnerPlayer;}
    FBProtocol::ITEM_TYPE   GetWeaponType()		 { return mInfo.WeaponType; }
    bool					GetIsActive()		 { mInfo.Lock_IsActive.LockRead();  bool active = mInfo.IsActive;  mInfo.Lock_IsActive.UnlockRead();    return active; }


};


