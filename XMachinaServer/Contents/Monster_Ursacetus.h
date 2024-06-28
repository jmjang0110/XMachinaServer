#pragma once
#include "GameMonster.h"


/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// GameEntity
///     ก่
///	GameObject
///		ก่
/// GameMonster
///		ก่
/// Monster_Ursacetus
/// 
/// -------------------------------+
/// Monster_Onyscidus
///	Monster_AdvancedCombatDroid_5
/// 
/// 
/// 

class Monster_Ursacetus : public GameMonster
{
private:
	float speed{}; /* TEST */

public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	Monster_Ursacetus();
	Monster_Ursacetus(UINT32 sessionID);
	virtual ~Monster_Ursacetus();

};

