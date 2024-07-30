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


	//TEST 
	Coordinate sI = SectorController::GetSectorIdxByPosition(Vec3(25.f, 0.f, 260.f));
	SPtr<GameMonster> monster = std::make_shared<GameMonster>();
;	auto trans = monster->AddComponent<Transform>(ComponentInfo::Type::Transform);
	monster->AddComponent<Collider>(ComponentInfo::Type::Collider);

	trans->SetPosition(Vec3(25.f, 0.f, 260.f));
	monster->SetOwnerNPCController(this);
	int ID = 7777; // test 
	monster->SetID(ID);
	monster->SetMonsterType(FBProtocol::MONSTER_TYPE_ADVANCED_COMBAT_DROIR_5);
	
	sc->AddMonsterInSector(sI, ID, monster);
	AddMonster(ID, monster);

	monster->UpdateSnapShot();


	const std::vector<SPtr<GameMonster>>* EnemyPrototypes = RESOURCE_MGR->GetBattleScene()->GetEnemies();
	for (int i = 0; i < EnemyPrototypes->size(); ++i) {

		SPtr<GameMonster> enemy = (*EnemyPrototypes)[i]->Clone(); // 복사본을 만든다 ( 원본은 건들이지 않는다. )
		enemy->SetOwnerNPCController(this);
		AddMonster(enemy->GetID(), enemy);
		Coordinate SectorIndex = SectorController::GetSectorIdxByPosition(enemy->GetTransform()->GetPosition());
		sc->AddMonsterInSector(SectorIndex, enemy->GetID(), enemy);
		
		enemy->UpdateSnapShot();
		enemy->Start();

		//break;
	}

	return;


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

					//monster->AddScript<Script_Enemy>(ScriptInfo::Type::Enemy);
					//monster->AddScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

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

