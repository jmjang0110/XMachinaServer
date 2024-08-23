#pragma once
#include "Script_EnemyStat.h"

class Script_EnemyController;
class Script_Enemy : public Script_EnemyStat
{
protected:
	struct AttackType{
		static constexpr int None        = 0;
		static constexpr int BasicAttack = 1;

		static constexpr int _count		 = 2;
	};

protected:
	SPtr<Script_EnemyController>	mEnemyController	= {};
	float							mAnimTime			= {};
	int								mCurrAttackStep		= {};

	Vec3							mSpawnPos			= {};
	Vec3							mSpawnRot			= {};
	std::string						mPherosInfo			= {};

public:
	Script_Enemy();
	Script_Enemy(SPtr<GameObject> owner);
	virtual ~Script_Enemy();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);

	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void End();

	/// +------------------------------
	///		Stat :  virtual function 
	/// ------------------------------+

	virtual void StartAttack();
	virtual bool Attack();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr<GameObject> instigator = nullptr) override;

protected:
	virtual void AttackCallback();
	virtual void AttackEndCallback();
	virtual void DeathEndCallback();

	void OnExitFromViewList();


public:
	int GetCurrAttackStep()							{ return mCurrAttackStep; }
	SPtr<Script_EnemyController> GetController()	{ return mEnemyController; }


};

