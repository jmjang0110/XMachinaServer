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
private:
	float mMaxHP        = {};
	float mShieldAmount = {};

	float mCrntHP       = {};
	float mPrevHP       = {};

	float mPhero		= {};
	bool  mIsDead       = {};

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

	/* Set */
	void SetMaxHP(float hp)		 { mMaxHP = hp; }
	void SetShield(float shield) { mShieldAmount = shield; }
	void AddShield(float shield) { mShieldAmount += shield; }

	/* Get */
	bool	IsDead() const { return mIsDead; }
	float	GetCrntHp() const { return mCrntHP; }
	float	GetMaxHp() const { return mMaxHP; }
	float GetPhero() const { return mPhero; }
public:
	bool UpdatePrevHP() { bool res = mPrevHP == mCrntHP; mPrevHP = mCrntHP; return res; }


};

