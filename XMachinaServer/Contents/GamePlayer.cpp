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
#include "Script_Phero.h"
#include "Script_PheroDropper.h"
#include "Script_PlayerStat.h"




GamePlayer::GamePlayer()
	: GameObject(-1)
{
}

GamePlayer::GamePlayer(UINT32 sessionID, SPtr_GameSession owner)
	: GameObject(sessionID)
{

	GameObject::SetType(GameObjectInfo::Type::GamePlayer);

	mSnapShot.ID       = sessionID;
	mSnapShot.Owner    = owner;

}

GamePlayer::~GamePlayer()
{
	mSnapShot.Owner = nullptr;

	for (int i = 0; i < mBullets.size(); ++i) {
		mBullets[i] = nullptr;
	}

	for (int i = 0; i < mSkills.size(); ++i) {
		mSkills[i] = nullptr;
	}

	mVlist.Clear();
}

void GamePlayer::Update()
{
	GameObject::Update(); // CPkt_PlayerTRnasform 패킷을 받을 때만 Update 
	
	/* Update View List */
	Vec3  Pos        = GetTransform()->GetPosition();
	float ViewRange  = mViewRangeRadius;
	mOwnerPC->GetOwnerRoom()->GetSectorController()->UpdateViewList(this, Pos, ViewRange);
	
	CollideCheckWithMonsters();


}

void GamePlayer::WakeUp()
{
	GameObject::WakeUp();

}

void GamePlayer::Start()
{
	GameObject::Start();

	mPlayerStat = GetScript<Script_PlayerStat>(ScriptInfo::Type::Stat).get();
	/// +-------------------------------------------------------------------------------
	///		CREATE GAME BULLETS
	/// -------------------------------------------------------------------------------+
	for (int i = 0; i < GameObjectInfo::maxBulletsNum; ++i) {
		
		SPtr<GameBullet> bullet = MEMORY->Make_Shared<GameBullet>(i, std::dynamic_pointer_cast<GamePlayer>(shared_from_this()));
		
		bullet->AddComponent<Transform>(ComponentInfo::Type::Transform);
		bullet->AddComponent<Collider>(ComponentInfo::Type::Collider); 
		bullet->AddScript<Script_Bullet>(ScriptInfo::Type::Bullet);

		mBullets[i] = bullet;
		mPossibleBulletIndex.push(i);
	}


	/// +-------------------------------------------------------------------------------
	///		CREATE GAME SKILLS
	/// -------------------------------------------------------------------------------+
	for (int i = 0; i < FBProtocol::PLAYER_SKILL_TYPE_END; ++i) {
		SPtr<GameSkill> skill = MEMORY->Make_Shared<GameSkill>(i, std::dynamic_pointer_cast<GamePlayer>(shared_from_this()));
		
		FBProtocol::PLAYER_SKILL_TYPE skilltype = static_cast<FBProtocol::PLAYER_SKILL_TYPE>(i);
		skill->SetSkillType(skilltype);
		switch (skilltype)
		{
		case FBProtocol::PLAYER_SKILL_TYPE_CLOACKING: {
			skill->SetCoolTime(3.f);
			skill->SetDurationTime(0.f);
		}
		break;
		case FBProtocol::PLAYER_SKILL_TYPE_IR_DETECTOR:{
			skill->SetCoolTime(10.f);
			skill->SetDurationTime(30.f);
		}
		break;
		case FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL:{
			skill->SetCoolTime(2.f);
			skill->SetDurationTime(10.f);
		}
		break;
		case FBProtocol::PLAYER_SKILL_TYPE_SHIELD:{
			skill->SetCoolTime(2.f);
			skill->SetDurationTime(4.5f);
		}
		break;
		default:
			break;
		};
		mSkills[i] = skill;
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
	mVlist.Clear();


	mSnapShot.Lock_IsExit.LockWrite();
	mSnapShot.IsExit = true;
	mSnapShot.Lock_IsExit.UnlockWrite();


}

int GamePlayer::OnShoot(Vec3& ray)
{
	return -1;

	//int possibleIndex = -1;
	//if (mPossibleBulletIndex.try_pop(possibleIndex)) {
	//	
	//	if (0 <= possibleIndex && possibleIndex < GameObjectInfo::maxBulletsNum) {
	//		
	//		mBullets[possibleIndex]->GetTransform()->SetPosition(GetTransform()->GetSnapShot().GetPosition());
	//		mBullets[possibleIndex]->SetOnShootDir(ray);
	//		mBullets[possibleIndex]->SetWeaponType(S_GetCurrWeapon());	// 총알 종류 설정 
	//		mBullets[possibleIndex]->Activate();							// PQCS - Register Update !
	//		
	//		return possibleIndex;
	//	}
	//}

	//return -1;
}

int GamePlayer::OnHitEnemy(int32_t monster_id, Vec3& ray)
{
	int possibleIndex = -1;
	if (mPossibleBulletIndex.try_pop(possibleIndex)) {

		if (0 <= possibleIndex && possibleIndex < GameObjectInfo::maxBulletsNum) {

			mBullets[possibleIndex]->GetTransform()->SetPosition(GetTransform()->GetSnapShot().GetPosition());
			mBullets[possibleIndex]->SetOnShootDir(ray);
			mBullets[possibleIndex]->SetHitMonsterID(monster_id);
			mBullets[possibleIndex]->SetWeaponType(S_GetCurrWeapon());	// 총알 종류 설정 
			mBullets[possibleIndex]->Activate();						// PQCS - Register Update !

			return possibleIndex;
		}
	}

	return -1;
}

bool GamePlayer::OnSkill(FBProtocol::PLAYER_SKILL_TYPE type)
{
	GameSkill::State skillState = mSkills[type]->S_GetState();
	switch (skillState)
	{
	case GameSkill::State::Impossible:
	{
		return false;
	}
		break;
	case GameSkill::State::Possible:
	{
		float currPhero = S_GetPhero();
		bool res = mSkills[type]->OnSkill(currPhero);
		if (res == false)
			return false;
	}
		break;
	case GameSkill::State::Active:
	{
		return false;
	}
		break;
	default:
		assert(0);
		break;
	}

	return true;
}

void GamePlayer::UpdateViewList(std::vector<SPtr<GamePlayer>> players, std::vector<SPtr<GameMonster>> monster)
{
	ViewList ViewList_Prev;
	mSnapShot.Lock_VList_SnapShot.LockWrite();
	ViewList_Prev            = mSnapShot.VList_SnapShot;;
	mSnapShot.VList_SnapShot = mVlist;
	mSnapShot.Lock_VList_SnapShot.UnlockWrite();

	std::vector<SPtr<GameMonster>> NewMonsters;
	std::vector<SPtr<GameMonster>> RemoveMonsters;

	/// +--------------------------------------------------------------------------------
	///	1. [PLAYER] VIEW LIST 
	/// ---------------------------------------------------------------------------------+
	for (int i = 0; i < players.size(); ++i) {
		mVlist.TryInsertPlayer(players[i]->GetID(), players[i]);
	}



	/// +--------------------------------------------------------------------------------
	///	2. [NEW MONSTER] VIEW LIST 
	/// ---------------------------------------------------------------------------------+
	for (int i = 0; i < monster.size(); ++i) {
		Script_Stat::ObjectState objState = monster[i]->S_GetObjectState();
		if (objState == Script_Stat::ObjectState::End)
			continue;

		bool IsSuccess = mVlist.TryInsertMonster(monster[i]->GetID(), monster[i]);
		if (IsSuccess) {
			// 새로 들어옴
			NewMonsters.push_back(monster[i]);
			//LOG_MGR->Cout("[ ", monster[i]->GetID(), " ] : NewMonsters \n");
		}
	}

	std::unordered_set<int> currentMonsterIDs;
	for (int i = 0; i < monster.size(); ++i) {
		currentMonsterIDs.insert(monster[i]->GetID());
	}


	/// +--------------------------------------------------------------------------------
	///	3. [REMOVE MONSTER] VIEW LIST 
	/// ---------------------------------------------------------------------------------+
	for (auto& it : ViewList_Prev.VL_Monsters) {
		// 이전 ViewList에 있던 Monster가 현재 ViewList에 없다면 
		if (currentMonsterIDs.find(it.first) == currentMonsterIDs.end()) {
			mVlist.RemoveMonster(it.first);
			RemoveMonsters.push_back(it.second);

			//LOG_MGR->Cout("[ ", it.first, " ] : ", it.second, " : DeActivate\n");
		}
	}


	/// +------------------------------------------------------------------------------------------------------
	/// 4. SEND NEW MOSNTERS PACKET 
	/// ------------------------------------------------------------------------------------------------------+
	if (NewMonsters.size() > 0) {

		auto NewMonster_spkt = FBS_FACTORY->SPkt_NewMonster(NewMonsters);
		GetSessionOwner()->Send(NewMonster_spkt);

		for (int i = 0; i < NewMonsters.size(); ++i) {

			/// > ▷ SEND MOSNTER STATE 
			auto						script         = NewMonsters[i]->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
			FBProtocol::MONSTER_BT_TYPE btType         = script->GetMonsterBTType();
			const auto&					SPkt_monState  = FBS_FACTORY->SPkt_Monster_State(NewMonsters[i]->GetID(), btType);
			GetSessionOwner()->Send(SPkt_monState);

			/* TARGET PACKET */
			SPtr<GameObject> target = script->GetTarget(); /* Lock Read */
			int targetplayer_id = 0;
			if (target && target->GetType() == GameObjectInfo::Type::GamePlayer) {
				targetplayer_id = target->GetID();
			}

			/// > ▷ BROADCAST MOSNTER TRAGET 
			int monster_id = NewMonsters[i]->GetID();
			auto pkt       = FBS_FACTORY->SPkt_Monster_Target(monster_id, targetplayer_id, -1);
			GAME_MGR->BroadcastRoom(mOwnerPC->GetOwnerRoom()->GetID(), pkt);
		}
		//LOG_MGR->Cout("SEND NEW MONSTER \n");
	}

	/// +------------------------------------------------------------------------------------------------------
	/// 5. SEND REMOVE MOSNTERS PACKET 
	/// ------------------------------------------------------------------------------------------------------+
	for (int i = 0; i < RemoveMonsters.size(); ++i) {
		const auto& RemoveMonster_serverPacket = FBS_FACTORY->SPkt_RemoveMonster(RemoveMonsters[i]->GetID());
		GetSessionOwner()->Send(RemoveMonster_serverPacket);
	}
}

void GamePlayer::CollideCheckWithBullets()
{
	

}

void GamePlayer::CollideCheckWithMonsters()
{    

	for (auto& iter : mVlist.VL_Monsters) {
		Script_Stat::ObjectState enemystate = iter.second->S_GetObjectState();

		switch (enemystate)
		{
		case Script_Stat::ObjectState::Deactive:	// Not In Any Player View List 
		case Script_Stat::ObjectState::End:		// Enemy State - Dead -> Phero All Deactive -> Enemy State - End 
			continue;
		break;

		case Script_Stat::ObjectState::Active:
		{

		}
		break;
		case Script_Stat::ObjectState::Dead:
		{
			Vec3 playerPos = GetTransform()->GetSnapShot().GetPosition();

			// 몬스터의 Phero와 충돌체크  
			const std::vector<SPtr<GamePhero>>& pheros = iter.second->GetAllPheros();

			for (int i = 0; i < pheros.size(); ++i) {
				if (pheros[i]->IsSetTarget() == false)
					continue;

				//ColliderSnapShot SNS_Phero = pheros[i]->GetCollider()->GetSnapShot();
				Vec3 pheroPos = pheros[i]->GetTransform()->GetPosition();

				playerPos.y = 0.f; pheroPos.y = 0.f;
				bool IsCollide = COLLISION_MGR->CollideCheck(pheroPos, playerPos, 2.5f);
				if (IsCollide) {

					const auto& phero_scirpt         = pheros[i]->GetScript<Script_Phero>(ScriptInfo::Type::Phero);
					int			targetid             = phero_scirpt->GetTargetPlayerID();

					/// > ▷ Continue If Phero Already Get Target Player ID 
					if (targetid != -1) {
						continue;
					}

					/// > ▷ Player Get Phero 
					phero_scirpt->SetTargetPlayerID(GetID());
					mPlayerStat->AddPheroAmount(phero_scirpt->GetAmount());

					/// > ▷ Broadcast Phero's Target Player ID 
					int  pheroID  = pheros[i]->GetID();
					int  targetID = GetID();

					pheros[i]->OnTarget(); // IsSetTargetPlayer = true;

					auto pkt      = FBS_FACTORY->SPkt_GetPhero(pheroID, targetID);
					GAME_MGR->BroadcastRoom(mOwnerPC->GetOwnerRoom()->GetID(), pkt);

					LOG_MGR->Cout(phero_scirpt->GetLevel(), "[", pheroID, "]", " => Get Phero", "  -- [", targetID, "]\n");

				}

			}
		}
		break;

		default:
			assert(0);
			break;
		}
	}
}

void GamePlayer::Push_PossibleBulletIndex(int idx)
{
	mPossibleBulletIndex.push(idx);
}

float GamePlayer::S_GetHp()
{
	return mPlayerStat->S_GetHp();
}
