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


	GameObject::RegisterUpdate();

}
