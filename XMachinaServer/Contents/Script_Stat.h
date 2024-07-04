#pragma once
#include "Script.h"
#include "GameObject.h"


class Script_Stat : public Script	
{
private:
	float mMaxHP        = {};
	float mShieldAmount = {};

	float mCrntHP       = {};
	float mPrevHP       = {};

	bool  mIsDead       = {};



public:
	Script_Stat();
	Script_Stat(UINT32 id);
	~Script_Stat();

public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

	virtual bool Hit(float damage, GameObject* instigator = nullptr);
	virtual void Dead();


public:
	bool UpdatePrevHP() { bool res = mPrevHP == mCrntHP; mPrevHP = mCrntHP; return res; }

	/* Set */
	void SetMaxHP(float hp)		 { mMaxHP = hp; }
	void SetShield(float shield) { mShieldAmount = shield; }
	void AddShield(float shield) { mShieldAmount += shield; }

	/* Get */
	bool	IsDead() const { return mIsDead; }
	float	GetCrntHp() const { return mCrntHP; }
	float	GetMaxHp() const { return mMaxHP; }



};

