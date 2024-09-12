#include "pch.h"
#include "NPCController.h"

#include "Sector.h"
#include "SectorController.h"
#include "GameRoom.h"
#include "GameObject.h"
#include "ResourceManager.h"

NPCController::NPCController()
{
}

NPCController::~NPCController()
{
	for (auto& iter : mMonsters) {
		iter.second = nullptr;
	}
	for (auto& iter : mNPCs) {
		iter.second = nullptr;
	}
	for (auto& iter : mStaticItems) {
		iter.second = nullptr;
	}
	for (auto& iter : mDynamicItems) {
		iter.second = nullptr;
	}
}

void NPCController::Init(SPtr<GameRoom> owner)
{
	mOwnerRoom = owner;
	InitMonsters();
	InitItems();
}


void NPCController::InitMonsters()
{
	SectorController* sc = mOwnerRoom->GetSectorController();


	/// ___________________________________________________________________
	///							<SectorController>
	///						   ��					  ��
	///						  Sector				Sector
	/// ___________________________________________________________________
	///					      ��  ��  ��...			��
	/// ___________________________________________________________________
	/// <NPCController> --> [1][2][3][4]....[5]...[EnemyPrototypes.size()]	<--- ( Cloens )
	/// ___________________________________________________________________

	int monster_id = 1;
	const std::vector<SPtr<GameObject>>* EnemyPrototypes = RESOURCE_MGR->GetBattleScene()->GetEnemies();
	for (int i = 0; i < EnemyPrototypes->size(); ++i) {

		SPtr<GameObject> enemy = (*EnemyPrototypes)[i]->Clone(); // ���纻�� ����� ( ������ �ǵ����� �ʴ´�. )
		// NPC
		AddMonster(enemy->GetID(), enemy);
		Coordinate SectorIndex = SectorController::GetSectorIdxByPosition(enemy->GetTransform()->GetPosition());
		sc->AddMonsterInSector(SectorIndex, enemy->GetID(), enemy);

		enemy->SetOwnerRoom(mOwnerRoom);
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
		SPtr<GameObject> d_Item = (*dynamicItemPrototypes)[i]->Clone();

		AddDynamicItem(d_Item->GetID(), d_Item);

		// INIT
		d_Item->Start();
		d_Item->SetOwnerRoom(mOwnerRoom);
	}


	for (int i = 0; i < staticItemPrototypes->size(); ++i) {
		auto item = (*staticItemPrototypes)[i];
		SPtr<GameObject> s_Item = item->Clone();

		AddStaticItem(s_Item->GetID(), s_Item);


		// INIT
		s_Item->Start();
		s_Item->SetOwnerRoom(mOwnerRoom);
	}

}

void NPCController::AddMonster(UINT32 id, SPtr<GameObject> monster)
{
	const auto& iter = mMonsters.find(id);
	if (iter == mMonsters.end()) {
		mMonsters.insert(std::make_pair(id, monster));
	}
	
}

void NPCController::AddDynamicItem(UINT32 id, SPtr<GameObject> item)
{
	const auto& iter = mDynamicItems.find(id);
	if (iter == mDynamicItems.end()) {
		mDynamicItems.insert(std::make_pair(id, item));
	}
}

void NPCController::AddStaticItem(UINT32 id, SPtr<GameObject> item)
{
	const auto& iter = mStaticItems.find(id);
	if (iter == mStaticItems.end()) {
		mStaticItems.insert(std::make_pair(id, item));
	}
}

SPtr<GameObject> NPCController::GetMonster(UINT32 monsterID)
{
	const auto& iter = mMonsters.find(monsterID);
	if (iter == mMonsters.end()) {
		return nullptr;
	}

	return iter->second;
}

SPtr<GameObject> NPCController::GetStaticItem(UINT32 item_id)
{

	const auto& iter = mStaticItems.find(item_id);
	if (iter == mStaticItems.end()) {
		return nullptr;
	}

	return iter->second;
}

SPtr<GameObject> NPCController::GetDynamicItem(UINT32 item_id)
{
	const auto& iter = mDynamicItems.find(item_id);
	if (iter == mDynamicItems.end()) {
		return nullptr;
	}

	return iter->second;
}

SPtr<GameObject> NPCController::GetItem(UINT32 item_id)
{
	SPtr<GameObject> item = nullptr;
	item = GetDynamicItem(item_id);
	if (item)
		return item;

	item = GetStaticItem(item_id);
	return item;
}



