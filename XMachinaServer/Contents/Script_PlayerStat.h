#pragma once

#include "Script_Stat.h"

class Script_PlayerStat : public Script_Stat
{
private:

public:

	Script_PlayerStat();
	Script_PlayerStat(UINT32 id);
	~Script_PlayerStat();


public:
	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;

	virtual void Attack();
	virtual void Death();
	virtual bool Hit(float damage, GameObject* instigator = nullptr) override;

public:

};

