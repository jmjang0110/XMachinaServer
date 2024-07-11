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


struct MonsterInfo 
{
	uint32_t			ID;
	MonsterType			Type;		/*	���� ����	*/
	
	/* Stat Script �� ����... */
	float				HP;			/*		HP		*/
	float				Attack;		/*	  ���ݷ�		*/

	/* ... Transform Component�� ���� */
	Vec3				Position;
	Vec3				Rotation;

	Vec3				SpineDir;

};

class GameMonster : public GameObject
{
private:
	Coordinate  mSectorIndex;   // ��� ���Ϳ� �����ִ°�?
	MonsterInfo mInfo;			// ���� ���� 


public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster ���� ���̵� - (�����ǰ� �Ҹ�� �� ���� �ӽ� ���̵�)*/
	virtual ~GameMonster();

};

