#pragma once

#include "GameObject.h"
#include "DB_Monster.h"
#include "ObjectSnapShot.h"

/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// Monster
///    ��
///	Monster_Ursacetus
///	Monster_Onyscidus
///	Monster_AdvancedCombatDroid_5
/// -------------------------------+

class NPCController;

struct MonsterSnapShot : public ObjectSnapShot
{
	MonsterType			Type;		/*	���� ����	*/
	
	/* Stat Script �� ����... */
	float			    HP;			Lock::SRWLock lock_HP;/*		HP		*/
	float				Attack;		Lock::SRWLock lock_Attack;/*	  ���ݷ�		*/

	Vec3				Position;	Lock::SRWLock lock_Position;
	Vec3				Rotation;	Lock::SRWLock lock_Rotation;
	Vec3				SpineDir;	Lock::SRWLock lock_SpineDir;
};

class GameMonster : public GameObject
{
private:
	NPCController* mOwnerNC;

	Coordinate  mSectorIndex;   // ��� ���Ϳ� �����ִ°�?
	Lock::SRWLock mLock_SnapShot;
	MonsterSnapShot mInfo;			// ���� ���� 
	
	std::atomic_int mActivate_Ref = 0;


public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

	virtual SPtr<GameMonster> Clone();

public:
	void SetMonsterID(uint32_t id)				{ mInfo.ID			= id;}
	void SetMonsterType(MonsterType type)		{ mInfo.Type		= type;}
	void SetAttack(float attack)				{ mInfo.Attack		= attack; }
		
	void SetHP(float hp)						{ mInfo.lock_HP.LockWrite();		mInfo.HP               = hp;			mInfo.lock_HP.UnlockWrite(); }
	void SetPosition(Vec3 pos)					{ mInfo.lock_Position.LockWrite();	mInfo.Position         = pos;			mInfo.lock_Position.UnlockWrite(); }
	void SetRotation(Vec3 rot)					{ mInfo.lock_Rotation.LockWrite();	mInfo.Rotation         = rot;			mInfo.lock_Rotation.UnlockWrite(); }
	void SetSpineDir(Vec3 spinedir)				{ mInfo.lock_SpineDir.LockWrite();	mInfo.SpineDir         = spinedir;		mInfo.lock_SpineDir.UnlockWrite(); }

	// Get �Լ���
	uint32_t		GetMonsterID()			{	return mInfo.ID;		}
	MonsterType		GetMonsterType(){	return mInfo.Type;		}
	float			GetAttack()		{	return mInfo.Attack;	}
	float			GetHP()			{ mInfo.lock_HP.LockRead();  float hp = mInfo.HP;	mInfo.lock_HP.UnlockRead(); return hp; }
	Vec3			GetPosition()	{ mInfo.lock_Position.LockRead(); Vec3 pos = mInfo.Position; mInfo.lock_Position.UnlockRead(); return pos; }

	void			SetOwnerNPCController(NPCController* nc)	{ mOwnerNC = nc; }
	NPCController*	GetOwnerNPCController()						{ return mOwnerNC; }
	int				GetActivate_RefCnt()						{ return mActivate_Ref.load(); }
	void			DecreaseRef()								{ mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }

	MonsterSnapShot GetSnapShot() { mLock_SnapShot.LockWrite(); MonsterSnapShot snapShot = mInfo; mLock_SnapShot.UnlockWrite(); return snapShot; }

public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~GameMonster();
	 
};

