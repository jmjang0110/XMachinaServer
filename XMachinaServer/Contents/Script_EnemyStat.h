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
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();


	/// +------------------------------
	///		Stat :  virtual function 
	/// ------------------------------+
	virtual void Attack();
	virtual void AttackCallback();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

public:

	// Getters
	int GetEnemyLevel() const				 { return mEnemyLevel; }
	int GetPheroLevel() const				 { return mPheroLevel; }
											 
	float GetMoveSpeed() const				 { return mMoveSpeed; }
	float GetRotationSpeed() const			 { return mRotationSpeed; }
	
	float GetAttackRotationSpeed() const	 { return mAttackRotationSpeed; }
	float GetDetectionRange() const			 { return mDetectionRange; }
	
	float GetAttackRate() const				 { return mAttackRate; }
	float GetAttackRange() const			 { return mAttackRange; }
	float GetAttackCoolTime() const			 { return mAttackCoolTime; }

	// Setters
	void SetEnemyLevel(int level)			 { mEnemyLevel = level; }
	void SetPheroLevel(int level)			 { mPheroLevel = level; }
	
	void SetMoveSpeed(float speed)			 { mMoveSpeed = speed; }
	void SetRotationSpeed(float speed)		 { mRotationSpeed = speed; }
	
	void SetAttackRotationSpeed(float speed) { mAttackRotationSpeed = speed; }
	void SetDetectionRange(float range)		 { mDetectionRange = range; }
	
	void SetAttackRate(float rate)			 { mAttackRate = rate; }
	void SetAttackRange(float range)		 { mAttackRange = range; }
	void SetAttackCoolTime(float time)	     { mAttackCoolTime = time; }
};

