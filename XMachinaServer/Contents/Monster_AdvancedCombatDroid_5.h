#pragma once

#include "GameMonster.h"

class Monster_AdvancedCombatDroid_5 : public GameMonster
{
private:
	float speed = 0.f;



public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	Monster_AdvancedCombatDroid_5();
	Monster_AdvancedCombatDroid_5(UINT32 sessionID);
	~Monster_AdvancedCombatDroid_5();


};

