#pragma once

#include "GameObject.h"
#include "DB_Monster.h"

/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// Monster
///    ↑
///	Monster_Ursacetus
///	Monster_Onyscidus
///	Monster_AdvancedCombatDroid_5
/// -------------------------------+


class GameMonster : public GameObject
{
private:
	MonsterType			mType;		/* 몬스터 종류*/
	float				mHP;		/* HP */
	float				mAttack;	/* 공격력 */

public:
	GameMonster();
	GameMonster(UINT32 id); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	~GameMonster();

};

