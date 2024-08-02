#pragma once
#include "Script_Stat.h"

/// +----------------------------------------------------
///					Script_EnemyStat
/// ----------------------------------------------------+
///
///		�� Read Only ( Set at Loading )
/// 
/// ______________________________________________________


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
	
	std::string	mAttack1AnimName     = {};
	std::string	mAttack2AnimName     = {};
	std::string	mAttack3AnimName     = {};
	std::string mGetHitAnimName      = {};
	std::string	mDeathAnimName       = {};



public:
	Script_EnemyStat();
	Script_EnemyStat(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_EnemyStat();


public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp();
	virtual bool Start();
	virtual bool Update();
	virtual void OnDestroy();

	/// +------------------------------
	///		Stat :  virtual function 
	/// ------------------------------+
	virtual bool Attack();
	virtual void AttackCallback();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

public:

	// Getters
	int			GetStat_EnemyLevel() const					{ return mEnemyLevel; }
	int			GetStat_PheroLevel() const					{ return mPheroLevel; }
	float		GetStat_MoveSpeed() const					{ return mMoveSpeed; }
	float		GetStat_RotationSpeed() const				{ return mRotationSpeed; }
	float		GetStat_AttackRotationSpeed() const			{ return mAttackRotationSpeed; }
	float		GetStat_DetectionRange() const				{ return mDetectionRange; }
	float		GetStat_AttackRate() const					{ return mAttackRate; }
	float		GetStat_AttackRange() const					{ return mAttackRange; }
	float		GetStat_AttackCoolTime() const				{ return mAttackCoolTime; }
	std::string GetStat_Attack1AnimName()					{ return mAttack1AnimName; }
	std::string GetStat_Attack2AnimName()					{ return mAttack2AnimName; }
	std::string GetStat_Attack3AnimName()					{ return mAttack3AnimName; }
	std::string GetStat_GetHitAnimName()					{ return mGetHitAnimName; }
	std::string GetStat_DeathAnimName()						{ return mDeathAnimName; }

	// Setters
	void		SetStat_EnemyLevel(int level)				{ mEnemyLevel          = level; }
	void		SetStat_PheroLevel(int level)				{ mPheroLevel          = level; }
	void		SetStat_MoveSpeed(float speed)				{ mMoveSpeed           = speed; }
	void		SetStat_RotationSpeed(float speed)			{ mRotationSpeed       = speed; }
	void		SetStat_AttackRotationSpeed(float speed)	{ mAttackRotationSpeed = speed; }
	void		SetStat_DetectionRange(float range)			{ mDetectionRange      = range; }
	void		SetStat_AttackRate(float rate)				{ mAttackRate          = rate;  }
	void		SetStat_AttackRange(float range)			{ mAttackRange         = range; }
	void		SetStat_AttackCoolTime(float time)			{ mAttackCoolTime      = time;  }
	void		SetStat_Attack1AnimName(std::string name)	{ mAttack1AnimName     = name;  }
	void		SetStat_Attack2AnimName(std::string name)	{ mAttack2AnimName     = name;  }
	void		SetStat_Attack3AnimName(std::string name)	{ mAttack3AnimName     = name;  }
	void		SetStat_GetHitName(std::string name)		{ mGetHitAnimName      = name;  }
	void		SetStat_DeathAnimName(std::string name)		{ mDeathAnimName       = name;  }
};

