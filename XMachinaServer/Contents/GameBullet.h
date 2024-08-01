#pragma once

#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "ObjectSnapShot.h"
#include "DB_Weapon.h"
class GameBullet;
class GamePlayer;
struct  BulletSnapShot : public ObjectSnapShot
{
	GamePlayer*				PlayerOwner       = nullptr;
	SPtr<GameBullet>		BulletOwner       = nullptr;

	FBProtocol::WEAPON_TYPE	WeaponType        = FBProtocol::WEAPON_TYPE_MIN;	Lock::SRWLock Lock_WeaponType;
	bool					IsActive          = false;							Lock::SRWLock Lock_IsActive;
};

class GameBullet : public GameObject	
{

private:
	SPtr<GamePlayer> mOwnerPlayer = nullptr;

private:
	BulletSnapShot mInfo;

	std::atomic_int mActivate_Ref = 0;


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


public:
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }

public:
	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetWeaponType(FBProtocol::WEAPON_TYPE type)		{ mInfo.Lock_WeaponType.LockWrite(); mInfo.WeaponType = type; mInfo.Lock_WeaponType.UnlockWrite(); }

	// Getters
	int						GetActivate_RefCnt() { return mActivate_Ref.load(); }
    SPtr<GamePlayer>		GetOwnerPlayer()	{ return mOwnerPlayer;}
    FBProtocol::WEAPON_TYPE GetWeaponType()		{ mInfo.Lock_WeaponType.LockRead();   auto type = mInfo.WeaponType;  mInfo.Lock_WeaponType.UnlockRead();   return type; }
    bool					GetIsActive()		{ mInfo.Lock_IsActive.LockRead();  bool active = mInfo.IsActive;  mInfo.Lock_IsActive.UnlockRead();    return active; }
};


