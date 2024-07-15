#include "pch.h"
#include "GamePlayer.h"
#include "PlayerController.h"
#include "Sector.h"
#include "SectorController.h"
#include "NPCController.h"

#include "GameRoom.h"

GamePlayer::GamePlayer()
	: GameObject(-1)
{
}

GamePlayer::GamePlayer(UINT32 sessionID, SPtr_GameSession owner)
	: GameObject(sessionID)
{

	GameObject::SetType(GameObjectInfo::Type::GamePlayer);

	mInfo.PlayerID = sessionID;
	mInfo.Owner    = owner;
}

GamePlayer::~GamePlayer()
{
	mInfo.Owner = nullptr;
}

void GamePlayer::Update()
{
	GameObject::Update();

	/* Update View List */
	mOwnerPC->GetOwnerRoom()->GetSectorController()->UpdateViewList(this, mInfo.Position, mInfo.ViewRangeRadius);
	
}

void GamePlayer::WakeUp()
{
	GameObject::WakeUp();

}

void GamePlayer::Start()
{
	GameObject::Start();

}

void GamePlayer::Activate()
{
	GameObject::Activate();

}

void GamePlayer::DeActivate()
{
	GameObject::DeActivate();

}

void GamePlayer::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}

void GamePlayer::UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> montser)
{
	mInfo.VList_Prev = mInfo.Vlist;
	//mInfo.Vlist.Clear();

	for (int i = 0; i < players.size(); ++i) {
		mInfo.Vlist.TryInsertPlayer(players[i]->GetID(), players[i]);
	}

	for (int i = 0; i < montser.size(); ++i) {
		bool IsSuccess = mInfo.Vlist.TryInsertMonster(montser[i]->GetID(), montser[i]);
		if (IsSuccess) {
			// 새로 들어옴
		}
	}

	std::unordered_set<int> currentMonsterIDs;
	for (int i = 0; i < montser.size(); ++i) {
		currentMonsterIDs.insert(montser[i]->GetID());
	}

	for (auto& it : mInfo.VList_Prev.VL_Monsters) {
		// 이전 ViewList에 있던 Monster가 현재 ViewList에 없다면 
		if (currentMonsterIDs.find(it.first) == currentMonsterIDs.end()) {
			mInfo.Vlist.RemoveMonster(it.first);
			LOG_MGR->Cout(it.second, " : DeActivate\n");
		}
	}

}
