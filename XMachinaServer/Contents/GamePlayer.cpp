#include "pch.h"
#include "GameRoom.h"
#include "GamePlayer.h"
#include "GameSession.h"
#include "GameMonster.h"


#include "FBsPacketFactory.h"

#include "PlayerController.h"
#include "SectorController.h"
#include "NPCController.h"
#include "GameBullet.h"

#include "Sector.h"
#include "Script_Bullet.h"
#include "CollisionManager.h"
#include "Script_Phero.h"

#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "PlayerController.h"

#include "Script_DefaultEnemyBT.h"
#include "Script_EnemyController.h"




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

	/// +-------------------------------------------------------------------------------
	///		CREATE GAME BULLETS
	/// -------------------------------------------------------------------------------+
	for (int i = 0; i < GameObjectInfo::maxBulletsNum; ++i) {
		auto owner = std::dynamic_pointer_cast<GamePlayer>(shared_from_this());
		SPtr<GameBullet> bullet = MEMORY->Make_Shared<GameBullet>(i, owner);
		bullet->AddComponent<Transform>(ComponentInfo::Type::Transform);
		bullet->AddComponent<Collider>(ComponentInfo::Type::Collider); // �浹üũ �׳� �Ÿ� ���� ���� ���� 
		bullet->AddScript<Script_Bullet>(ScriptInfo::Type::Bullet);

		mInfo.Bullets[i] = bullet;
		mInfo.mPossibleBulletIndex.push(i);
	}

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

int GamePlayer::OnShoot()
{
	int possibleIndex = -1;
	if (mInfo.mPossibleBulletIndex.try_pop(possibleIndex)) {
		
		if (0 <= possibleIndex && possibleIndex < GameObjectInfo::maxBulletsNum) {
			mInfo.Bullets[possibleIndex]->RegisterUpdate(); // PQCS 
			return possibleIndex;
		}
	}

	return -1;
}

void GamePlayer::UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> monster)
{
	mInfo.VList_Prev = mInfo.Vlist;
	std::vector<MonsterSnapShot> NewMonsters;
	std::vector<MonsterSnapShot> RemoveMonsters;
	std::vector<SPtr<GameMonster>> NewMonsters_Objects;

	for (int i = 0; i < players.size(); ++i) {
		mInfo.Vlist.TryInsertPlayer(players[i]->GetID(), players[i]);
	}

	for (int i = 0; i < monster.size(); ++i) {
		bool IsSuccess = mInfo.Vlist.TryInsertMonster(monster[i]->GetID(), monster[i]);
		if (IsSuccess) {
			// ���� ���� 
			MonsterSnapShot snapShot = monster[i]->GetSnapShot();
			NewMonsters.push_back(snapShot);
			NewMonsters_Objects.push_back(monster[i]);

			LOG_MGR->Cout("[ ", snapShot.ID , " ] : NewMonsters \n");

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

			LOG_MGR->Cout("[ ", it.first, " ] : ", it.second, " : DeActivate\n");
		}
	}


	/// +------------------------------------------------------------------------------------------------------
	///		SEND NEW / REMOVE MOSNTERS PACKET 
	/// ------------------------------------------------------------------------------------------------------+
	/* Send New Mosnter Packet */
	if (NewMonsters.size() > 0) {
		// [BSH] : ���ο� ���Ͱ� �÷��̾� �� �ϳ��� �� ����Ʈ�� ������ ��� Ŭ�󿡰� ��ε� ĳ���� �ؾ� �Ѵ�.
		auto NewMonster_serverPacket = FBS_FACTORY->SPkt_NewMonster(NewMonsters);
		GAME_MGR->BroadcastRoom(mOwnerPC->GetOwnerRoom()->GetID(), NewMonster_serverPacket);
		//const auto& NewMonster_serverPacket = FBS_FACTORY->SPkt_NewMonster(NewMonsters);
		//GetSessionOwner()->Send(NewMonster_serverPacket);

		for (int i = 0; i < NewMonsters_Objects.size(); ++i) {

			/* MONSTER STATE */
			auto script                          = NewMonsters_Objects[i]->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
			FBProtocol::MONSTER_BT_TYPE btType   = script->GetMonsterBTType();
			const auto& MonsterType_serverPacket = FBS_FACTORY->SPkt_Monster_State(NewMonsters_Objects[i]->GetID(), btType);
			GetSessionOwner()->Send(MonsterType_serverPacket);


			/* TARGET PACKET */
			// [BSH] : Ÿ���� ��� 0�� �����ֵ��� �ؾ� �ϸ� ��� �÷��̾�� ��ε� ĳ���� �ؾ��Ѵ�.
			SPtr<GameObject> target = script->GetTarget();
			int targetplayer_id{};
			if (target) {
				targetplayer_id = target->GetID();
			}

			if (target->GetType() == GameObjectInfo::Type::GamePlayer) {
				int monster_id = NewMonsters_Objects[i]->GetID();
				//const auto& pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, targetplayer_id, -1);
				//GetSessionOwner()->Send(pkt);

				auto pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, targetplayer_id, -1);
				GAME_MGR->BroadcastRoom(mOwnerPC->GetOwnerRoom()->GetID(), pkt);
			}

			//if (target) {
			//	int targetplayer_id = target->GetID();
			//	if (target->GetType() == GameObjectInfo::Type::GamePlayer) {
			//		int monster_id = NewMonsters_Objects[i]->GetID();
			//		const auto& pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, targetplayer_id, -1);
			//		GetSessionOwner()->Send(pkt);
			//	}
			//}
		}
		LOG_MGR->Cout("SEND NEW MONSTER \n");

	}
	
	/* Send Remove Monster Packet */
	for (int i = 0; i < RemoveMonsters.size(); ++i) {
		const auto& RemoveMonster_serverPacket = FBS_FACTORY->SPkt_RemoveMonster(RemoveMonsters[i].ID);
		GetSessionOwner()->Send(RemoveMonster_serverPacket);
	}
}

void GamePlayer::CollideCheckWithBullets()
{
	

}

void GamePlayer::CollideCheckWithMonsters()
{
	ColliderSnapShot SNS_Player = GetCollider()->GetSnapShot();

	for (auto& iter : mInfo.Vlist.VL_Monsters) {
		if (iter.second->IsActive() == false)
			continue;

		// ����ִ� ���� 
		if (iter.second->GetHP() > 0.f)
		{
			//iter.second->GetMotionState() // ���� ���¶�� 
			ColliderSnapShot SNS_Monster = iter.second->GetCollider()->GetSnapShot();
			bool IsCollide = COLLISION_MGR->CollideCheck(SNS_Player, SNS_Monster);
			if (IsCollide) {
				// �÷��̾��� HP �� ������ 

			}

		}
		// ���� ���� 
		else {
			// ������ Phero�� �浹üũ  
			const std::vector<SPtr<GameObject>>& pheros = iter.second->GetAllPheros();

			for (int i = 0; i < pheros.size(); ++i) {
				ColliderSnapShot SNS_Phero = pheros[i]->GetCollider()->GetSnapShot();
				bool IsCollide = COLLISION_MGR->CollideCheck(SNS_Phero, SNS_Player);
				if (IsCollide) {
					const auto& phero_scirpt = pheros[i]->GetScript<Script_Phero>(ScriptInfo::Type::Phero);
					int targetid = phero_scirpt->GetTargetPlayerID();

				/*	if (phero_script->GetState() == �̵���)
						coninue;*/

					if (targetid = -1) {
						phero_scirpt->SetTargetPlayerID(GetID());
						// �÷��̾��� ��μ�ġ��  �ø���. 

						auto pkt = FBS_FACTORY->SPkt_GetPhero(pheros[i]->GetID(), GetID());
						GAME_MGR->BroadcastRoom(mOwnerPC->GetOwnerRoom()->GetID(), pkt, GetID());
					}
				}

			}
		}


		
	}
}

