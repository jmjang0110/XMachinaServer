#include "pch.h"
#include "BTTaskM_CheckDetectionRange.h"
#include "BTTask.h"
#include "BTNode.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "FBsPacketFactory.h"
#include "ResourceManager.h"
#include "NPCController.h"
#include "PlayerController.h"
#include "SectorController.h"
#include "RoomManager.h"
#include "GameRoom.h"

#include "Script_Player.h"
#include "Script_Skill.h"

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	if (!mEnemyController->GetPaths()->empty()) {
		return BTNodeState::Failure;
	}

	SPtr<GameObject> target = FindDetectionPlayer();
	if (nullptr == target) {
		mEnemyController->RemoveAllAnimation();
		mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
		mEnemyController->SetTarget(nullptr);
		return BTNodeState::Failure;
	}

	if (target) {
		auto player_entity    = target->GetScriptEntity<Script_Player>();
		auto cloaking_entity  = player_entity->GetSkillEntity(FBProtocol::PLAYER_SKILL_TYPE_CLOACKING);

		if (cloaking_entity->GetCurrSkillState() == SkillState::Active) {
			mEnemyController->SetTarget(nullptr);
			mEnemyController->RemoveAllAnimation();
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
			return BTNodeState::Failure;
		}
	}

	mEnemyController->SetTarget(target);
	MonsterBTTask::mAnimation->GetController()->SetValue("walk", true);

	return BTNodeState::Success;
}

MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)
{
}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;
}

SPtr<GameObject> MonsterTask::CheckDetectionRange::FindDetectionPlayer()
{
	Vec3 EnemyPos                                  = mOwner->GetTransform()->GetPosition();
	PlayerController* PC                           = mOwner->GetOwnerRoom()->GetPlayerController();
	std::vector<std::pair<UINT32, Vec3>> playerPos = PC->GetPlayersPosition(); /* Lock */

	int closestPlayerID = -1;
	float minDistance = std::numeric_limits<float>::max();

	for (int i = 0; i < playerPos.size(); ++i) {

		UINT32	ID   = playerPos[i].first;
		Vec3	Pos  = playerPos[i].second;

		// distance -> Player ---- Monster 
		float distance = Vec3::Distance(Pos, EnemyPos);

		// *  IF Player In Monster Detection Range && 
		// *  Find Nearest Player ID . 
		if (distance  <= mStat->GetStat_DetectionRange() &&  distance < minDistance) {
			minDistance = distance;
			closestPlayerID = ID;

		}
	}

	SPtr<GameObject> target = nullptr;
	if (closestPlayerID != -1) {
		target = PC->GetPlayer(closestPlayerID);
		auto player_entity = target->GetScriptEntity<Script_Player>();
		if (player_entity->GetCurrState() == PlayerState::Exit) {
			target = nullptr;
		}
	}

	return target;
}



