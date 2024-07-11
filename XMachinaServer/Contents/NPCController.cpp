#include "pch.h"
#include "NPCController.h"

#include "Sector.h"
#include "SectorController.h"
#include "GameRoom.h"

#include "Monster_AdvancedCombatDroid_5.h"
#include "Monster_Ursacetus.h"
#include "Monster_Onyscidus.h"


NPCController::NPCController()
{
}

NPCController::~NPCController()
{
}

void NPCController::Init(SPtr_GameRoom owner)
{
	mOwnerRoom = owner;

	SectorController* sc = mOwnerRoom->GetSectorController();
	Coordinate maxSectorIdx = sc->GetMaxSectorIndex();

	InitMonsters(maxSectorIdx);
	InitBullets();
	InitNPCs();

}


void NPCController::InitMonsters(Coordinate maxSectorIdx)
{
	SectorController* sc = mOwnerRoom->GetSectorController();


	// 섹터에 몬스터를 미리 생성시킨다.
	for (int i = 0; i < maxSectorIdx.z; ++i) {
		for (int k = 0; k < maxSectorIdx.x; ++k) {
			Coordinate idx = Coordinate(i, k);
			SectorInfo::Type SectorType = sc->GetSectorType(idx);

			switch (SectorType)
			{
			case SectorInfo::Type::None:
			{

			}
			break;
			case SectorInfo::Type::Monsters:
			{

			}
			break;
			case SectorInfo::Type::Boss:
			{

			}
			break;
			}
			
		}
	}
}


void NPCController::InitBullets()
{
}

void NPCController::InitNPCs()
{
}

SPtr<GameMonster> NPCController::CreateMonster(UINT32 id, Coordinate sectorIdx, MonsterType monType)
{
	SPtr<GameMonster> monster;
	
	switch (monType)
	{
	case MonsterType::Ursacetus:
	{
		monster = std::make_shared<Monster_Ursacetus>(id, sectorIdx);
		
	}
	break;
	case MonsterType::Onyscidus:
	{
		monster = std::make_shared<Monster_Onyscidus>(id, sectorIdx);

	}
	break;
	case MonsterType::AdvancedCombatDroid_5:
	{
		monster = std::make_shared<Monster_AdvancedCombatDroid_5>(id, sectorIdx);

	}
	break;
	};

	return monster;
}
