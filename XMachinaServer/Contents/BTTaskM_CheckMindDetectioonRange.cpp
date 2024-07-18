#include "pch.h"
#include "BTTask.h"

/// +-------------------------------------------------------------------------
///	> ������ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	// ���� ����� ���� Ÿ������ ����
	if (!SetTargetNearestEnemy()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Running;
	}

	// ��� ��ã�Ⱑ �������̰ų� ���� ���� ���� ���� ��� ���� ���� ����
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

