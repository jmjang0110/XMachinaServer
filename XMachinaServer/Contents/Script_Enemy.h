#pragma once
#include "Script_EnemyStat.h"
#include "Script_EnemyController.h"

class Script_Enemy : public Script_EnemyStat
{
private:
	Script_EnemyController* mEnemyController = {};
	

public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	virtual void OnDestroy() override;

	virtual void Attack();
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;


};

