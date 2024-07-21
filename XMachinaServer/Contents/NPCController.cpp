#include "pch.h"
#include "NPCController.h"

#include "Sector.h"
#include "SectorController.h"
#include "GameRoom.h"


#include "GameMonster.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Ursacetus.h"
#include "Script_Onyscidus.h"

#include "ResourceManager.h"


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

	int monster_id = 1;

	const std::vector<SPtr<GameMonster>>* EnemyPrototypes = RESOURCE_MGR->GetBattleScene()->GetEnemies();
	for (int i = 0; i < EnemyPrototypes->size(); ++i) {
		SPtr<GameMonster> enemy = (*EnemyPrototypes)[i]->Clone(); // 복사본을 만든다 ( 원본은 건들이지 않는다. )
		enemy->SetOwnerNPCController(this);
		AddMonster(enemy->GetID(), enemy);
		Coordinate SectorIndex = SectorController::GetSectorIdxByPosition(enemy->GetTransform()->GetPosition());
		sc->AddMonsterInSector(SectorIndex, enemy->GetID(), enemy);
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
		monster = std::make_shared<GameMonster>(id, sectorIdx);
		// AddSCript 
		
	}
	break;
	case MonsterType::Onyscidus:
	{
		monster = std::make_shared<GameMonster>(id, sectorIdx);

	}
	break;
	case MonsterType::AdvancedCombatDroid_5:
	{
		monster = std::make_shared<GameMonster>(id, sectorIdx);

	}
	break;
	};

	return monster;
}

void NPCController::AddMonster(UINT32 id, SPtr<GameMonster> monster)
{
	const auto& iter = mMonsters.find(id);
	if (iter == mMonsters.end()) {
		mMonsters.insert(std::make_pair(id, monster));
	}
	
}

