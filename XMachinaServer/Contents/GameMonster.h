#pragma once

#include "GameObject.h"
#include "DB_Monster.h"

/// +-------------------------------
///		     Game Monster
/// ________________________________
///
/// Monster
///    ��
///	Monster_Ursacetus
///	Monster_Onyscidus
///	Monster_AdvancedCombatDroid_5
/// -------------------------------+


class GameMonster : public GameObject
{
private:
	MonsterType			mType;		/* ���� ����*/
	float				mHP;		/* HP */
	float				mAttack;	/* ���ݷ� */

public:
	virtual void Update() override;

public:
	GameMonster();
	GameMonster(UINT32 id); /* Monster ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~GameMonster();

};

