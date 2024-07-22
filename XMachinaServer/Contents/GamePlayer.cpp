#include "pch.h"
#include "GameRoom.h"
#include "GamePlayer.h"
#include "GameSession.h"

#include "FBsPacketFactory.h"

#include "PlayerController.h"
#include "SectorController.h"
#include "NPCController.h"

#include "Sector.h"

GamePlayer::GamePlayer()
	: GameObject(-1)
{
}

GamePlayer::GamePlayer(UINT32 sessionID, SPtr_GameSession owner)
	: GameObject(sessionID)
{

	GameObject::SetType(GameObjectInfo::Type::GamePlayer);

	mInfo.ID       = sessionID;
	mInfo.Owner    = owner;
}

GamePlayer::~GamePlayer()
{
	mInfo.Owner = nullptr;
}

void GamePlayer::Update()
{
	GameObject::Update(); // CPkt_PlayerTRnasform ��Ŷ�� ���� ���� Update 

	
	/* Update View List */
	mOwnerPC->GetOwnerRoom()->GetSectorController()->UpdateViewList(this, mInfo.Position, mInfo.ViewRangeRadius);
	
	//LOG_MGR->Cout("POS : ", GetTransform()->GetWorldTransform()._41, " ", GetTransform()->GetWorldTransform()._42, " ", GetTransform()->GetWorldTransform()._43 ," \n");
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

void GamePlayer::Exit()
{
	/* Exit Room Clear Data */
	mInfo.Vlist.Clear();

}

void GamePlayer::UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> monster)
{
	mInfo.VList_Prev = mInfo.Vlist;
	std::vector<MonsterSnapShot> NewMonsters;
	std::vector<MonsterSnapShot> RemoveMonsters;

	for (int i = 0; i < players.size(); ++i) {
		mInfo.Vlist.TryInsertPlayer(players[i]->GetID(), players[i]);
	}

	for (int i = 0; i < monster.size(); ++i) {
		bool IsSuccess = mInfo.Vlist.TryInsertMonster(monster[i]->GetID(), monster[i]);
		if (IsSuccess) {
			// ���� ���� 
			MonsterSnapShot snapShot = monster[i]->GetSnapShot();
			NewMonsters.push_back(snapShot);
		}
	}

	std::unordered_set<int> currentMonsterIDs;
	for (int i = 0; i < monster.size(); ++i) {
		currentMonsterIDs.insert(monster[i]->GetID());
	}

	for (auto& it : mInfo.VList_Prev.VL_Monsters) {
		// ���� ViewList�� �ִ� Monster�� ���� ViewList�� ���ٸ� 
		if (currentMonsterIDs.find(it.first) == currentMonsterIDs.end()) {
			mInfo.Vlist.RemoveMonster(it.first);

			MonsterSnapShot snapShot = it.second->GetSnapShot();
			RemoveMonsters.push_back(snapShot);

			LOG_MGR->Cout(it.second, " : DeActivate\n");
		}
	}


	/// +------------------------------------------------------------------------------------------------------
	///		SEND NEW / REMOVE MOSNTERS PACKET 
	/// ------------------------------------------------------------------------------------------------------+
	/* Send New Mosnter Packet */
	if (NewMonsters.size() > 0) {
		const auto& NewMonster_serverPacket = FBS_FACTORY->SPkt_NewMonster(NewMonsters);
		GetSessionOwner()->Send(NewMonster_serverPacket);
	}
	
	/* Send Remove Monster Packet */
	for (int i = 0; i < RemoveMonsters.size(); ++i) {
		const auto& RemoveMonster_serverPacket = FBS_FACTORY->SPkt_RemoveMonster(RemoveMonsters[i].ID);
		GetSessionOwner()->Send(RemoveMonster_serverPacket);
	}
}

