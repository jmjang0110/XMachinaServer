#pragma once
#include "Script_EnemyStat.h"
#include "Script_EnemyController.h"

class Script_Enemy : public Script_EnemyStat
{
private:
	SPtr<Script_EnemyController> mEnemyController = {};

public:
	Script_Enemy();
	Script_Enemy(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Enemy();


public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	/* Runtime */
	virtual SPtr<Component> Clone(SPtr<GameObject> copyOwner) const;

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

};

