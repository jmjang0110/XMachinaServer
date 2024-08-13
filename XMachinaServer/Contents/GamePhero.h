#pragma once

#include "GameObject.h"
#include "ObjectSnapShot.h"


class GamePhero;
struct PheroSnapShot : public ObjectSnapShot
{
	SPtr<GamePhero>	PheroOwner = nullptr;
	int				ID         = -1;
	int				PheroLevel = -1;
	Vec3			OffsetDist;

	Vec3			Position; Lock::SRWLock Lock_Position;
	UINT32			TargetPlayerID = -1; Lock::SRWLock Lock_TargetPlayerID; 

	void UpdateID(int monsterID, int pheroIndex) { ID = monsterID * 15 /* 현재 페로 생성 최대 개수 : 15*/ + pheroIndex; }

};

class GamePhero : public GameObject
{
private:
	PheroSnapShot mInfo;

	std::atomic_int mActivate_Ref = 0;

	bool IsSetTargetPlayer = false;

public:
	GamePhero();
	GamePhero(UINT32 id);
	virtual ~GamePhero();
	 
public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

	virtual SPtr<GamePhero> Clone();

public:
	void SetPosition(Vec3 pos)			{ mInfo.Lock_Position.LockWrite();	mInfo.Position = pos;			mInfo.Lock_Position.UnlockWrite(); }
	Vec3 GetPosition()					{ mInfo.Lock_Position.LockRead(); Vec3 pos = mInfo.Position; mInfo.Lock_Position.UnlockRead(); return pos; }
	int	 GetActivate_RefCnt()			{ return mActivate_Ref.load(); }

	void OnTarget()						{ IsSetTargetPlayer = true; }
	bool IsSetTarget()					{ return IsSetTargetPlayer; }

	void SetOffsetDist(Vec3 dist)		{ mInfo.OffsetDist = dist; }
	Vec3 GetOffsetDist()				{ return mInfo.OffsetDist; }
};

