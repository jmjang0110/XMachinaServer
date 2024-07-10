#pragma once
#include "Script_EnemyStat.h"
#include "Script_EnemyController.h"

class Script_Enemy : public Script_EnemyStat
{
private:
	Script_EnemyController* mEnemyController = {};
	

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
	virtual void Dead();
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;


};

