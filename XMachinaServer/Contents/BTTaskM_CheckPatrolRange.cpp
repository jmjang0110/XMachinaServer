#include "pch.h"
#include "BTTask.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Patrol Range 
/// __________________________________________________________________________


BTNodeState MonsterTask::CheckPatrolRange::Evaluate()
{
	mEnemyController->SetState(EnemyInfo::State::Walk);
	//GetOwner()->GetAnimation()->GetController()->SetValue("Walk", true);

	constexpr float adjRange = 0.2f;
	float dis = Vec3::Distance(mBaryCenter, GetOwner()->GetTransform()->GetPosition().xz());
	if (dis <= mPatrolRange + adjRange) {
		GetOwner()->GetAnimation()->GetController()->SetValue("Return", true);
		return BTNodeState::Running;
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckPatrolRange::CheckPatrolRange(SPtr_GameObject owner, const Vec3& baryCenter, float patrolRange)
	: BTTask(owner, BTTaskType::MonT_CheckPatrolRange)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

	mBaryCenter = baryCenter;
	mPatrolRange = patrolRange;

}

MonsterTask::CheckPatrolRange::CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckPatrolRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

}

MonsterTask::CheckPatrolRange::~CheckPatrolRange()
{
	mEnemyController = nullptr;

}