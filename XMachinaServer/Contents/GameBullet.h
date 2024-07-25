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
	FBProtocol::WEAPON_TYPE	WeaponType        = FBProtocol::WEAPON_TYPE_MIN; Lock::SRWLock Lock_WeaponType;
	bool					IsActive          = false; Lock::SRWLock Lock_IsActive;
	
	/// +-----------------------------------------------------------
	///		Transform 
	/// -----------------------------------------------------------+
	float					Velocity = {}; // READ ONLY 
	Vec3					Position = {};	Lock::SRWLock Lock_Position;
	Vec3					Rotation = {};	Lock::SRWLock Lock_Rotation;
	Vec3					MoveDir  = {};	Lock::SRWLock Lock_MoveDir;



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
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

	virtual SPtr<GameBullet> Clone();

public:
	int				GetActivate_RefCnt() { return mActivate_Ref.load(); }
	void			DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }

public:
	void SetPosition(Vec3 pos)								{ mInfo.Lock_Position.LockWrite();	mInfo.Position = pos;			mInfo.Lock_Position.UnlockWrite(); }
	void SetRotation(Vec3 rot)								{ mInfo.Lock_Rotation.LockWrite();	mInfo.Rotation = rot;			mInfo.Lock_Rotation.UnlockWrite(); }
	void SetMoveDir(Vec3 movedir)							{ mInfo.Lock_MoveDir.LockWrite();	mInfo.MoveDir  = movedir;		mInfo.Lock_MoveDir.UnlockWrite(); }
	void SetWeaponType(FBProtocol::WEAPON_TYPE type)		{ mInfo.Lock_WeaponType.LockWrite(); mInfo.WeaponType = type; mInfo.Lock_WeaponType.UnlockWrite(); }

	// Getters
    // Getters
    Vec3 GetPosition()							{ mInfo.Lock_Position.LockRead(); Vec3 pos = mInfo.Position; mInfo.Lock_Position.UnlockRead();	return pos;}
    Vec3 GetRotation()							{ mInfo.Lock_Rotation.LockRead();Vec3 rot = mInfo.Rotation; mInfo.Lock_Rotation.UnlockRead();  return rot;}
    Vec3 GetMoveDir()							{ mInfo.Lock_MoveDir.LockRead();   Vec3 dir = mInfo.MoveDir;  mInfo.Lock_MoveDir.UnlockRead();   return dir; }
    FBProtocol::WEAPON_TYPE GetWeaponType()		{ mInfo.Lock_WeaponType.LockRead();   auto type = mInfo.WeaponType;  mInfo.Lock_WeaponType.UnlockRead();   return type; }
    bool GetIsActive()							{ mInfo.Lock_IsActive.LockRead();  bool active = mInfo.IsActive;  mInfo.Lock_IsActive.UnlockRead();    return active; }
    SPtr<GamePlayer> GetOwnerPlayer()			{ return mOwnerPlayer;}
};


