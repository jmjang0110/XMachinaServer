#pragma once
#include "Script.h"
#include "GameObject.h"

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

class Script_Stat : public Script	
{
private:
	float mMaxHP        = {};
	float mShieldAmount = {};

	float mCrntHP       = {};
	float mPrevHP       = {};

	float mPhero		= {};

private:
	bool  mIsDead       = false; Lock::SRWLock Lock_IsDead;
	float mHP           = {}; Lock::SRWLock Lock_HP;

public:
	Script_Stat();
	Script_Stat(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Stat();

public:

	/// +------------------------------
	///			virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;

	virtual bool WakeUp()	override;
	virtual bool Start()	override;

	/// +------------------------------
	///		 Stat : virtual function 
	/// ------------------------------+
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr);
	virtual void Dead();


public:


	/// +------------------------------
	///			S E T T E R
	/// ------------------------------+
	void	SetMaxHP(float hp)			{ mMaxHP         = hp; }
	void	SetShield(float shield)		{ mShieldAmount  = shield; }
	void	AddShield(float shield)		{ mShieldAmount += shield; }

	void	SetSNS_HP(float hp)			{ Lock_HP.LockWrite();		mHP     = hp; Lock_HP.UnlockWrite(); }
	void	SetSNS_IsDead(bool isdead)	{ Lock_IsDead.LockWrite();	mIsDead = isdead; Lock_IsDead.UnlockWrite(); }

	/// +------------------------------
	///			G E T T E R
	/// ------------------------------+
	float	GetCrntHp() const			{ return mCrntHP; }
	float	GetMaxHp()	const			{ return mMaxHP; }
	float	GetPhero()	const			{ return mPhero; }

	bool	GetSNS_IsDead()				{ Lock_IsDead.LockRead();	 bool isdead = mIsDead; Lock_IsDead.UnlockRead();  return isdead; }
	float	GetSNS_HP()					{ Lock_HP.LockRead();		float hp     = mHP; Lock_HP.UnlockRead(); return hp; }
public:
	bool UpdatePrevHP()					{ bool res = mPrevHP == mCrntHP; mPrevHP = mCrntHP; return res; }


};

