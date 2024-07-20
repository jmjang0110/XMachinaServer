#include "pch.h"
#include "BTTask.h"
#include "Script_EnemyController.h"
#include "Script_EnemyStat.h"

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Patrol 
/// __________________________________________________________________________

BTNodeState MonsterTask::Patrol::Evaluate()
{
	Vec3 wayPoint = mWayPoints[mCurrWayPointIdx];

	const float kMinDistance = 1.f;
	const Vec3 toWayPoint = wayPoint - GetOwner()->GetTransform()->GetPosition().xz();

	if (toWayPoint.Length() > kMinDistance) {
		GetOwner()->GetTransform()->RotateTargetAxisY(wayPoint, mStat->GetStat_RotationSpeed());
		GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mPatrolSpeed * DELTA_TIME);
	}
	else {
		mCurrWayPointIdx = (mCurrWayPointIdx + 1) % mWayPoints.size();
	}

	return BTNodeState::Running;
}

MonsterTask::Patrol::Patrol(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Patrol, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat            = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

	mPatrolSpeed     = mStat->GetStat_MoveSpeed() * 0.5f;
}

MonsterTask::Patrol::~Patrol()
{
	mEnemyController = nullptr;

}

void MonsterTask::Patrol::SetWayPoints(std::vector<Vec3>&& wayPoints)
{
	mWayPoints = wayPoints;
}
