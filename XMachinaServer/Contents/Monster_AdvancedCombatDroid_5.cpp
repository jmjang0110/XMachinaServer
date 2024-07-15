#include "pch.h"
#include "Monster_AdvancedCombatDroid_5.h"
#include "TimeManager.h"
#include "ServerLib/MemoryManager.h"

Monster_AdvancedCombatDroid_5::Monster_AdvancedCombatDroid_5()
{
}

Monster_AdvancedCombatDroid_5::Monster_AdvancedCombatDroid_5(UINT32 sessionID, Coordinate sectorIdx)
	: GameMonster(sessionID, sectorIdx)
{
	GameObject::SetType(GameObjectInfo::Type::Monster_AdvancedCombat_5);

	GameMonster::SetID(sessionID);
	GameMonster::SetType(MonsterType::AdvancedCombatDroid_5);
	GameMonster::SetAttack(100);
	GameMonster::SetHP(100);
	GameMonster::SetPosition(Vec3(0.f, 0.f, 0.f));
	GameMonster::SetRotation(Vec3(0.f, 0.f, 0.f));
	GameMonster::SetSpineDir(Vec3(0.f, 0.f, 0.f));
}

Monster_AdvancedCombatDroid_5::~Monster_AdvancedCombatDroid_5()
{
}

void Monster_AdvancedCombatDroid_5::Update()
{
	GameMonster::Update();

	std::cout << "Monster_AdvancedCombatDroid_5\n";
	
	

}

void Monster_AdvancedCombatDroid_5::WakeUp()
{
}

void Monster_AdvancedCombatDroid_5::Activate()
{
}

void Monster_AdvancedCombatDroid_5::DeActivate()
{
}

void Monster_AdvancedCombatDroid_5::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);
	
	Update();


	if (GameMonster::GetActivate_RefCnt() > 0)
		GameObject::RegisterUpdate();

}
