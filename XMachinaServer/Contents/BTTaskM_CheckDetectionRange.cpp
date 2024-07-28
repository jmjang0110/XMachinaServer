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
///	> ▶▶▶ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	// [BSH] : 주변 타겟이 있다면 성공을 반환해야 하는데 이 전에는 타겟이 있는 경우 바로 실패를 반환한다.
	// 따라서 Attack이 계속 실행되지 않고 1회만 실행된다.
	// 때문에 타겟이 있는 경우 해당 타겟을 컨트롤러에서 가져와서 스킬을 검사하고 성공을 반환해야 한다.
	SPtr<GamePlayer> target = nullptr;
	if (!mEnemyController->GetTarget()) {
		target = FindDetectionPlayer();
		if (nullptr == target){
			mEnemyController->SetTarget(nullptr);
			return BTNodeState::Failure;
		}
		else {
			mEnemyController->SetTarget(target);
		}
	}
	else {
		// TODO : must use??? 
		target = std::dynamic_pointer_cast<GamePlayer>(mEnemyController->GetTarget());
	}

	if (true == target->GetActiveSkill(SkillInfo::Type::Cloaking)) {
		mEnemyController->SetTarget(nullptr);
		return BTNodeState::Failure;
	}

	mEnemyController->SetState(EnemyInfo::State::Walk);
	GetOwner()->GetAnimation()->GetController()->SetValue("walk", true);
	return BTNodeState::Success;

//	if (!mEnemyController->GetTarget()) {
//
//		// 1. Find Target Player 
//		SPtr<GamePlayer> target = FindDetectionPlayer();
//		if (target) {
//			// 2. Check Is Player On Cloacking Skill ( IF Cloacking On -- Pass... )
//			if (true == target->GetActiveSkill(SkillInfo::Type::Cloaking)) {
//				mEnemyController->SetTarget(nullptr);
//				return BTNodeState::Failure;
//			}
//			else {
//				mEnemyController->SetTarget(target);
//				mEnemyController->SetState(EnemyInfo::State::Walk);
//				GetOwner()->GetAnimation()->GetController()->SetValue("walk", true);
//				return BTNodeState::Success;
//			}
//		}
//	}
//
//
//	return BTNodeState::Failure;
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
	if (closestPlayerID != -1) {
		target = PC->GetPlayer(closestPlayerID);
		if (target->IsExit() == true) {
			target == nullptr;
		}
	}

	return target;
}



