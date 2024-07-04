#pragma once
#include "Script_Stat.h"


class Script_EnemyStat : public Script_Stat
{
private:

public:
	Script_EnemyStat();
	Script_EnemyStat(UINT32 id);
	~Script_EnemyStat();


public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	
	virtual void Attack();
	virtual void Death();
	virtual bool Hit(float damage, GameObject* instigator = nullptr) override;
public:


};

