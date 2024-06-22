#pragma once
#include "GameMonster.h"


/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// GameEntity
///     ��
///	GameObject
///		��
/// GameMonster
///		��
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
	float speed; /* TEST */

public:
	virtual void Update() override;




public:
	Monster_Ursacetus();
	Monster_Ursacetus(UINT32 id);
	virtual ~Monster_Ursacetus();

};

