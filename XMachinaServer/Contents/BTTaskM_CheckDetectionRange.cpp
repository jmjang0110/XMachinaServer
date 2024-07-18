#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	if (!mEnemyController->GetTargetObject()) {
		mEnemyController->SetTargetObject(mTarget);

	}

	///* 타겟한 플레이어가 은신 상태라면.. */
	const auto& playerScript = mEnemyController->GetTargetObject()->GetScript<Script_Player>(ScriptInfo::Type::Stat);
	Skill* cloacking = playerScript->GetSkill(SkillInfo::Type::Cloaking);
	if (cloacking->GetState() == SkillInfo::State::Active) {
		mEnemyController->SetTargetObject(nullptr);
		return BTNodeState::Failure;
	}

	// 경로 길찾기가 실행중이거나 감지 범위 내에 들어온 경우 다음 노드로 진행
	if ((GetOwner()->GetTransform()->GetPosition() - mEnemyController->GetTargetObject()->GetTransform()->GetPosition()).Length() < mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Success;
	}
	else {
		mEnemyController->SetTargetObject(nullptr);
	}

	return BTNodeState::Failure;
}



MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}



