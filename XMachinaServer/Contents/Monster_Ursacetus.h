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



public:
	Monster_Ursacetus();
	Monster_Ursacetus(UINT32 sessionID, Coordinate sectorIdx);
	virtual ~Monster_Ursacetus();

public:
	void Init();

public:
	virtual void Update() override;
	virtual void WakeUp() override;

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

};

