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
	InitItems();

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
		// SECTOR
		Coordinate SectorIndex = SectorController::GetSectorIdxByPosition(enemy->GetTransform()->GetPosition());
		sc->AddMonsterInSector(SectorIndex, enemy->GetID(), enemy);
		
		// INIT
		enemy->UpdateSnapShot();
		enemy->Start();
	}

	return;
}

void NPCController::InitItems()
{
	SectorController* sc = mOwnerRoom->GetSectorController();

	auto dynamicItemPrototypes = RESOURCE_MGR->GetBattleScene()->GetDynamicItems();
	auto staticItemPrototypes  = RESOURCE_MGR->GetBattleScene()->GetStaticItems();

	for (int i = 0; i < dynamicItemPrototypes->size(); ++i) {
		SPtr<GameItem> d_Item = (*dynamicItemPrototypes)[i]->Clone();

		d_Item->SetOwnerNPCController(this);
		AddDynamicItem(d_Item->GetID(), d_Item);

		// INIT
		d_Item->Start();
	}


	for (int i = 0; i < staticItemPrototypes->size(); ++i) {
		auto item = (*staticItemPrototypes)[i];
		SPtr<GameItem> s_Item = item->Clone();

		s_Item->SetOwnerNPCController(this);
		AddStaticItem(s_Item->GetID(), s_Item);


		// INIT
		s_Item->Start();
	}

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

void NPCController::AddDynamicItem(UINT32 id, SPtr<GameItem> item)
{
	const auto& iter = mDynamicItems.find(id);
	if (iter == mDynamicItems.end()) {
		mDynamicItems.insert(std::make_pair(id, item));
	}
}

void NPCController::AddStaticItem(UINT32 id, SPtr<GameItem> item)
{
	const auto& iter = mStaticItems.find(id);
	if (iter == mStaticItems.end()) {
		mStaticItems.insert(std::make_pair(id, item));
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

SPtr<GameItem> NPCController::GetStaticItem(UINT32 item_id)
{

	const auto& iter = mStaticItems.find(item_id);
	if (iter == mStaticItems.end()) {
		return nullptr;
	}

	return iter->second;
}

SPtr<GameItem> NPCController::GetDynamicItem(UINT32 item_id)
{
	const auto& iter = mDynamicItems.find(item_id);
	if (iter == mDynamicItems.end()) {
		return nullptr;
	}

	return iter->second;
}

SPtr<GameItem> NPCController::GetItem(UINT32 item_id)
{
	SPtr<GameItem> item = nullptr;
	item = GetDynamicItem(item_id);
	if (item)
		return item;

	item = GetStaticItem(item_id);
	return item;
}

