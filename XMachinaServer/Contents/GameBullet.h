#pragma once

#include "GameObject.h"
#include "ObjectSnapShot.h"
#include "DB_Weapon.h"
class GameBullet;
class GamePlayer;
struct  BulletSnapShot : public ObjectSnapShot
{
	GamePlayer*				PlayerOwner       = nullptr;
	SPtr<GameBullet>		BulletOwner       = nullptr;
	WeaponInfo::WeaponName	WeaponName        = WeaponInfo::WeaponName::None;
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
	void SetPosition(Vec3 pos) { mInfo.Lock_Position.LockWrite();	mInfo.Position = pos;			mInfo.Lock_Position.UnlockWrite(); }
	void SetRotation(Vec3 rot) { mInfo.Lock_Rotation.LockWrite();	mInfo.Rotation = rot;			mInfo.Lock_Rotation.UnlockWrite(); }
	void SetMoveDir(Vec3 movedir) { mInfo.Lock_MoveDir.LockWrite();	mInfo.MoveDir  = movedir;		mInfo.Lock_MoveDir.UnlockWrite(); }
};


