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


struct MonsterInfo 
{
	uint32_t			ID;
	MonsterType			Type;		/*	몬스터 종류	*/
	
	/* Stat Script 로 빼자... */
	float				HP;			/*		HP		*/
	float				Attack;		/*	  공격력		*/

	/* ... Transform Component로 빼자 */
	Vec3				Position;
	Vec3				Rotation;

	Vec3				SpineDir;

};

class GameMonster : public GameObject
{
private:
	Coordinate  mSectorIndex;   // 어느 섹터에 속해있는가?
	MonsterInfo mInfo;			// 몬스터 정보 


public:
	virtual void Update() override;
	virtual void WakeUp() override;
	virtual void Start();

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:
	GameMonster();
	GameMonster(UINT32 id, Coordinate sectorIdx); /* Monster 생성 아이디 - (생성되고 소멸될 때 까지 임시 아이디)*/
	virtual ~GameMonster();

};

