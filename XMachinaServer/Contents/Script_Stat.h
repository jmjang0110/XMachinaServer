#pragma once
#include "Script.h"
#include "GameObject.h"

/// +-------------------------------
///		     Script_Stat 
/// ________________________________
///	> [ ���� ] 
///		- PC, NPC �� ������ �����ϱ� ���� ��ũ��Ʈ�̴�.
///		- Script_Stat�� ��ӹ޾� �� Stat�� Script�� ���� �����Ѵ�.
/// ________________________________
///	> [ ���� ] 
///			 	   Script_Stat 
///				��			    �� 
///	Script_EnemyStat		Script_PlayerStat 
/// -------------------------------+

class Script_Stat : public Script	
{
public:
	enum class State : UINT8 {
		Deactive, Active, Dead, End, _count,
	};

private:
	float mMaxHP        = {};
	float mShieldAmount = {};

	float mCrntHP       = {};
	float mPrevHP       = {};

	float mPhero		= {};

private:
	State  mState       = State::Deactive; Lock::SRWLock Lock_State;
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

	virtual void Activate();
	virtual void DeActivate();

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

	void	SetSNS_HP(float hp)			{ Lock_HP.LockWrite();		mHP      = hp; Lock_HP.UnlockWrite(); }
	void	SetSNS_State(State state)   { Lock_State.LockWrite();	mState   = state; Lock_State.UnlockWrite(); }

	/// +------------------------------
	///			G E T T E R
	/// ------------------------------+
	float	GetCrntHp() const			{ return mCrntHP; }
	float	GetMaxHp()	const			{ return mMaxHP; }
	float	GetPhero()	const			{ return mPhero; }

	float	GetSNS_HP()					{ Lock_HP.LockRead();		float hp     = mHP; Lock_HP.UnlockRead(); return hp; }
	State	GetSNS_State()				{ Lock_State.LockRead();	State state  = mState;  Lock_State.UnlockRead(); return state; }


public:
	bool UpdatePrevHP()					{ bool res = mPrevHP == mCrntHP; mPrevHP = mCrntHP; return res; }


};

