#include "pch.h"
#include "BTTaskM_CheckDetectionRange.h"

#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"

#include "NPCController.h"
#include "PlayerController.h"
#include "SectorController.h"
#include "GameRoom.h"
#include "GameManager.h"

#include "FBsPacketFactory.h"



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

	if (target && target->GetType() == GameObjectInfo::Type::GamePlayer) {
		SPtr<GamePlayer> player = std::dynamic_pointer_cast<GamePlayer>(target);
		GameSkill::State IsCloakingOn = player->GetSkillState(FBProtocol::PLAYER_SKILL_TYPE_CLOACKING);
		if (IsCloakingOn == GameSkill::State::Active) {
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

MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{

}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}

SPtr<GamePlayer> MonsterTask::CheckDetectionRange::FindDetectionPlayer()
{
	Vec3 EnemyPos                                  = GetOwner()->GetTransform()->GetPosition();
	PlayerController* PC                           = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetPlayerController();
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

	SPtr<GamePlayer>	target = nullptr;
	if (closestPlayerID != -1) {
		target = PC->GetPlayer(closestPlayerID);
		if (target->IsExit() == true) {
			target = nullptr;
		}
	}

	return target;
}



