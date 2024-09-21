#pragma once
#include "Script_Entity.h"
/// +-------------------------------
///		     Script_Stat 
/// ________________________________
///	> [ 설명 ] 
///		- PC, NPC 의 스탯을 관리하기 위한 스크립트이다.
///		- Script_Stat을 상속받아 각 Stat을 Script로 따로 구현한다.
/// ________________________________
///	> [ 구조 ] 
///			 	   Script_Stat 
///				↓			    ↓ 
///	Script_EnemyStat		Script_PlayerStat 
/// -------------------------------+

class Script_Stat : public Script_Entity
{
public:
	enum class ObjectState : UINT8 {
		Deactive, Active, Dead, End, _count,
	};

private:
	/// ---------------- HP ----------------
	float mMaxHP        = { 1000.f }; // test
	float mShieldAmount = {};

	float mCrntHP       = {};
	float mPrevHP       = {};

	/// ---------------- PHero ----------------
	float mMaxPhero		= 1000.f;
	float mPhero		= 1000.f; Lock::SRWLock Lock_Phero;

private:
	ObjectState  mObjectState = ObjectState::Deactive; Lock::SRWLock Lock_State;
	float mHP           = {}; Lock::SRWLock Lock_HP;

public:
	Script_Stat();
	Script_Stat(SPtr<GameObject> owner, UINT32 key);
	virtual ~Script_Stat();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);

	virtual void Activate();
	virtual void DeActivate();

	virtual void Start();

	

	/// +------------------------------
	///		 Stat : virtual function 
	/// ------------------------------+
	virtual bool Hit(float damage, SPtr<GameObject> instigator = nullptr);
	virtual void Dead();


public:


	/// +------------------------------
	///			S E T T E R
	/// ------------------------------+
	void	SetMaxHP(float hp)			{ mMaxHP         = hp; }
	void	SetShield(float shield)		{ mShieldAmount  = shield; }
	void	AddShield(float shield)		{ mShieldAmount += shield; }

	void	S_SetHp(float hp)					{ Lock_HP.LockWrite();		mHP            = hp;	Lock_HP.UnlockWrite(); }
	void	S_SetObjectState(ObjectState state) { Lock_State.LockWrite();	mObjectState   = state; Lock_State.UnlockWrite(); }

	/// +------------------------------
	///			G E T T E R
	/// ------------------------------+
	float	GetCrntHp() const			{ return mCrntHP; }
	float	GetMaxHp()	const			{ return mMaxHP; }
	float	GetPhero()	const			{ return mPhero; }

	float	S_GetHp()					{ Lock_HP.LockRead();		float hp                = mHP;			 Lock_HP.UnlockRead(); return hp; }
	ObjectState	S_GetObjectState()		{ Lock_State.LockRead();	ObjectState ObjectState = mObjectState;  Lock_State.UnlockRead(); return mObjectState; }


public:
	bool UpdatePrevHP()					{ bool res = mPrevHP == mCrntHP; mPrevHP = mCrntHP; return res; }

	/// +-------------------------------------------
	///	 >> Phero
	/// -------------------------------------------+
	virtual void AddPheroAmount(float pheroAmount);
	virtual bool ReducePheroAmount(float pheroCost);

	void  S_SetCurrPheroAmount(float phero);
	float S_GetCurrPheroAmount();
};

