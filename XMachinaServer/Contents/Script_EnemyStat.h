#pragma once
#include "Script_Stat.h"


class Script_EnemyStat : public Script_Stat
{
private:
	/* LEVEL */
	int			mEnemyLevel          = {};
	int			mPheroLevel          = {};

	/* SPEED */
	float		mMoveSpeed           = {};
	float		mRotationSpeed       = {};
	float		mAttackRotationSpeed = {};
	
	/* DETECTION */
	float		mDetectionRange      = {};
	
	/* ATTACK */
	float		mAttackRate          = {};
	float		mAttackRange         = {};
	float		mAttackCoolTime      = {};

public:
	Script_EnemyStat();
	Script_EnemyStat(UINT32 id);
	~Script_EnemyStat();


public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	virtual void OnDestroy() override;

	virtual void Attack();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

public:


};

