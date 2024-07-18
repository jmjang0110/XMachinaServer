#include "pch.h"
#include "NPCController.h"

#include "Sector.h"
#include "SectorController.h"
#include "GameRoom.h"


#include "GameMonster.h"

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

	int monster_id = 1;

	// 섹터에 몬스터를 미리 생성시킨다.
	for (int i = 0; i < maxSectorIdx.z; ++i) {
		for (int k = 0; k < maxSectorIdx.x; ++k) {
			Coordinate sectorIdx = Coordinate(i, k);
			SectorInfo::Type SectorType = sc->GetSectorType(sectorIdx);


			switch (SectorType)
			{
			case SectorInfo::Type::Monsters:
			{
				// TEST : 섹터에 몬스터 두마리 생성 
				for (int i = 0; i < 2; ++i) {
					SPtr<GameMonster> monster = CreateMonster(monster_id, sectorIdx, MonsterType::Ursacetus);
					monster->AddComponent<Transform>(ComponentInfo::Type::Transform);
					monster->AddComponent<Collider>(ComponentInfo::Type::Collider);

					monster->SetOwnerNPCController(this);

					AddMonster(monster_id, monster);
					sc->AddMonsterInSector(sectorIdx, monster_id, monster);
					monster_id++;
				}

			}
			break;
			case SectorInfo::Type::Boss:
			{
				/* Boss */
				SPtr<GameMonster> monster = CreateMonster(monster_id, sectorIdx, MonsterType::Onyscidus);
				monster->AddComponent<Transform>(ComponentInfo::Type::Transform);
				monster->AddComponent<Collider>(ComponentInfo::Type::Collider);

				monster->SetOwnerNPCController(this);

				AddMonster(monster_id, monster);
				sc->AddMonsterInSector(sectorIdx, monster_id, monster);
				monster_id++;

				/* Monsters */

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

void NPCController::AddMonster(UINT32 id, SPtr<GameMonster> monster)
{
	const auto& iter = mMonsters.find(id);
	if (iter == mMonsters.end()) {
		mMonsters.insert(std::make_pair(id, monster));
	}
	
}

