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
	if (!mEnemyController->GetTarget()) {

		// 1. Find Target Player 
		SPtr<GamePlayer> target = FindDetectionPlayer();
		if (target) {
			// 2. Check Is Player On Cloacking Skill ( IF Cloacking On -- Pass... )
			if (true == target->GetActiveSkill(SkillInfo::Type::Cloaking)) {
				mEnemyController->SetTarget(nullptr);
				return BTNodeState::Failure;
			}
			else {
				mEnemyController->SetTarget(target);
				mEnemyController->SetState(EnemyInfo::State::Walk);
				GetOwner()->GetAnimation()->GetController()->SetValue("walk", true);
				return BTNodeState::Success;
			}
		}
	}


	return BTNodeState::Failure;
}



MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}

SPtr<GamePlayer> MonsterTask::CheckDetectionRange::FindDetectionPlayer()
{
	Vec3 EnemyPos                                  = GetOwner()->GetTransform()->GetPosition();
	PlayerController* PC                           = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetPlayerController();
	std::vector<std::pair<UINT32, Vec3>> playerPos = PC->GetPlayersPosition();

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
	if(closestPlayerID != -1)
		target = PC->GetPlayer(closestPlayerID);

	return target;
}



