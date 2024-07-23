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
///	> ������ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	bool IsMindControlled = mEnemyController->IsMindControlled();
	if (IsMindControlled == false) {
		if (!mEnemyController->GetTargetPlayer()) {
			// Target Player �� ���ٸ� 
				// Target �� ã�´�. 
				Vec3 EnemyPos = GetOwner()->GetTransform()->GetPosition();
				PlayerController* PC = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetPlayerController();
				std::vector<std::pair<UINT32, Vec3>> playerPos = PC->GetPlayersPosition();
				
				UINT32 closestPlayerID =  -1;
				float minDistance      = std::numeric_limits<float>::max();

				for (int i = 0; i < playerPos.size(); ++i) {
					
					UINT32 ID = playerPos[i].first;
					Vec3 Pos  = playerPos[i].second;

					// �� �� ������ �Ÿ� ���`
					float distance = Vec3::Distance(Pos, EnemyPos);

					// ���� ª�� �Ÿ��� ������ �÷��̾� ã��
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

	///* Ÿ���� �÷��̾ ���� ���¶��.. */
	if (IsMindControlled == false) {
		if (mTargetPlayer == nullptr)
			return BTNodeState::Failure;

		///* Ÿ���� �÷��̾ ���� ���¶��.. */
		bool IsCloakingOn = mEnemyController->GetTargetPlayer()->GetActiveSkill(SkillInfo::Type::Cloaking);
		if (IsCloakingOn == true) {
			mEnemyController->SetTargetPlayer(nullptr);
			return BTNodeState::Failure;
		}
	}

	// ��� ��ã�Ⱑ �������̰ų� ���� ���� ���� ���� ��� ���� ���� ����
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



