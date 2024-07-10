#pragma once

#include "Script_PlayerStat.h"

class Script_Player : public Script_PlayerStat
{
private:


public:
	virtual void Activate();
	virtual void DeActivate();

	virtual bool WakeUp()	override;
	virtual bool Start()	override;
	virtual bool Update()	override;
	virtual void OnDestroy() override;


	virtual void Attack()	override;
	virtual void Dead()		override;
	virtual bool Hit(float damage, SPtr_GameObject instigator = nullptr) override;

};

