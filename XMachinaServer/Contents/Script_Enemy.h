#pragma once
#include "Script_EnemyStat.h"
#include "Script_EnemyController.h"

class Script_Enemy : public Script_EnemyStat
{
protected:
	struct AttackType{
		static constexpr int None = 0;
		static constexpr int BasicAttack = 1;

		static constexpr int _count = 2;
	};

protected:
	SPtr<Script_EnemyController>	mEnemyController = {};
	float							mAnimTime{};
	int								mCurrAttackStep{};


public:
	Script_Enemy();
	Script_Enemy(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Enemy();


public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	/* Runtime */
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

	virtual void StartAttack();
	virtual bool Attack();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

protected:
	virtual void AttackCallback();
	virtual void AttackEndCallback();
	virtual void DeathEndCallback();

public:
	int GetCurrAttackStep() { return mCurrAttackStep; }
};

