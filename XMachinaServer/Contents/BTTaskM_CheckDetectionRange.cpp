#include "pch.h"
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

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	bool IsMindControlled = mEnemyController->IsMindControlled();
	if (IsMindControlled == false) {
		if (!mEnemyController->GetTargetPlayer()) {
			// Target Player 가 없다면 
				// Target 을 찾는다. 
				Vec3 EnemyPos = GetOwner()->GetTransform()->GetPosition();
				PlayerController* PC = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetPlayerController();
				std::vector<std::pair<UINT32, Vec3>> playerPos = PC->GetPlayersPosition();
				
				UINT32 closestPlayerID =  -1;
				float minDistance      = std::numeric_limits<float>::max();

				for (int i = 0; i < playerPos.size(); ++i) {
					
					UINT32 ID = playerPos[i].first;
					Vec3 Pos  = playerPos[i].second;

					// 두 점 사이의 거리 계산`
					float distance = Vec3::Distance(Pos, EnemyPos);

					// 가장 짧은 거리를 가지는 플레이어 찾기
					if (distance < minDistance) {
						minDistance = distance;
						closestPlayerID = ID;

					}
				}

				//LOG_MGR->Cout("Closest Player ID : ", closestPlayerID);
				mEnemyController->SetTargetPlayer(PC->GetPlayer(closestPlayerID));
				mTargetPlayer = PC->GetPlayer(closestPlayerID);
		}


	}
	else {
		if (!mEnemyController->GetTargetMonster())
			mEnemyController->SetTargetMonster(mTargetMonster);
	}

	///* 타겟한 플레이어가 은신 상태라면.. */
	if (IsMindControlled == false) {
		if (mTargetPlayer == nullptr)
			return BTNodeState::Failure;

		///* 타겟한 플레이어가 은신 상태라면.. */
		bool IsCloakingOn = mEnemyController->GetTargetPlayer()->GetActiveSkill(SkillInfo::Type::Cloaking);
		if (IsCloakingOn == true) {
			mEnemyController->SetTargetPlayer(nullptr);
			return BTNodeState::Failure;
		}
	}

	// 경로 길찾기가 실행중이거나 감지 범위 내에 들어온 경우 다음 노드로 진행
	Vec3 TargetPos;
	if (IsMindControlled == false) {
		TargetPos = mEnemyController->GetTargetPlayer()->GetPosition();
	}
	else {
		TargetPos = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetPosition();
	}

	const Vec3 OwnerPos = GetOwner()->GetTransform()->GetPosition();
	const float Length = (OwnerPos - TargetPos).Length();
	if ((GetOwner()->GetTransform()->GetPosition() - TargetPos).Length() < mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		GetOwner()->GetAnimation()->GetController()->SetValue("Walk", true);
		return BTNodeState::Success;
	}
	else {
		if (IsMindControlled == false)
			mEnemyController->SetTargetPlayer(nullptr);
		else
			mEnemyController->SetTargetMonster(nullptr);
	}

	return BTNodeState::Failure;
}



MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}



