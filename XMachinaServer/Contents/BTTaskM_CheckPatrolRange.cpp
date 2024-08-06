#include "pch.h"
#include "BTTaskM_CheckPatrolRange.h"

#include "BTTask.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Patrol Range 
/// __________________________________________________________________________


BTNodeState MonsterTask::CheckPatrolRange::Evaluate()
{
	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
	//mEnemyController->SetState(EnemyInfo::State::Walk);
	//MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);

	constexpr float adjRange = 0.2f;
	float dis = Vec3::Distance(mBaryCenter, MonsterBTTask::mTransform->GetPosition().xz());
	if (dis <= mPatrolRange + adjRange) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Return", true);
		return BTNodeState::Running;
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckPatrolRange::CheckPatrolRange(SPtr_GameObject owner, const Vec3& baryCenter, float patrolRange)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckPatrolRange)
{
	mBaryCenter = baryCenter;
	mPatrolRange = patrolRange;

}

MonsterTask::CheckPatrolRange::CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckPatrolRange, callback)
{
}

MonsterTask::CheckPatrolRange::~CheckPatrolRange()
{
}