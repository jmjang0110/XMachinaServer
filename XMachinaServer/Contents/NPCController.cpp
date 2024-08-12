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


	/// ___________________________________________________________________
	///							<SectorController>
	///						   ↓					  ↓
	///						  Sector				Sector
	/// ___________________________________________________________________
	///					      ↓  ↓  ↓...			↓
	/// ___________________________________________________________________
	/// <NPCController> --> [1][2][3][4]....[5]...[EnemyPrototypes.size()]	<--- ( Cloens )
	/// ___________________________________________________________________

	int monster_id = 1;
	const std::vector<SPtr<GameMonster>>* EnemyPrototypes = RESOURCE_MGR->GetBattleScene()->GetEnemies();
	for (int i = 0; i < EnemyPrototypes->size(); ++i) {

		SPtr<GameMonster> enemy = (*EnemyPrototypes)[i]->Clone(); // 복사본을 만든다 ( 원본은 건들이지 않는다. )
		
		// NPC
		enemy->SetOwnerNPCController(this);
		AddMonster(enemy->GetID(), enemy);

		if (enemy->GetType() == GameObjectInfo::Type::Montser_Deus_Phase_1)
			int i = 0;

		// SECTOR
		Coordinate SectorIndex = SectorController::GetSectorIdxByPosition(enemy->GetTransform()->GetPosition());
		enemy->SetSectorIndex(SectorIndex);
		sc->AddMonsterInSector(SectorIndex, enemy->GetID(), enemy);
		
		// INIT
		enemy->UpdateSnapShot();
		enemy->Start();
	}

	return;
}


void NPCController::InitBullets()
{
}

void NPCController::InitNPCs()
{
}

void NPCController::AddMonster(UINT32 id, SPtr<GameMonster> monster)
{
	const auto& iter = mMonsters.find(id);
	if (iter == mMonsters.end()) {
		mMonsters.insert(std::make_pair(id, monster));
	}
	
}

SPtr<GameMonster> NPCController::GetMonster(UINT32 monsterID)
{
	const auto& iter = mMonsters.find(monsterID);
	if (iter == mMonsters.end()) {
		return nullptr;
	}

	return iter->second;
}

