#include "pch.h"
#include "BTTask.h"

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	// 가장 가까운 적을 타겟으로 설정
	if (!SetTargetNearestEnemy()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Running;
	}

	// 경로 길찾기가 실행중이거나 감지 범위 내에 들어온 경우 다음 노드로 진행
	if ((GetOwner()->GetTransform()->GetPosition() 
		- mEnemyController->GetTargetObject()->GetTransform()->GetPosition()).Length() 
		< mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Success;
	}
	else {
		mEnemyController->SetTargetObject(nullptr);
	}

	return BTNodeState::Failure;
}

bool MonsterTask::CheckMindDetectionRange::SetTargetNearestEnemy()
{

	return true;
	
}



MonsterTask::CheckMindDetectionRange::CheckMindDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckMindDetectionRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckMindDetectionRange::~CheckMindDetectionRange()
{
	mEnemyController = nullptr;

}

