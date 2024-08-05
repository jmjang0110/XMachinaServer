#include "pch.h"
#include "BTTaskM_Patrol.h"

#include "BTTask.h"
#include "Script_EnemyController.h"
#include "Script_EnemyStat.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
#include "ServerLib/ThreadManager.h"

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Patrol 
/// __________________________________________________________________________

BTNodeState MonsterTask::Patrol::Evaluate()
{
	if (mWayPoints.size() == 0)
		return BTNodeState::Failure;

	Vec3 wayPoint = mWayPoints[mCurrWayPointIdx];

	const float kMinDistance = 1.f;
	const Vec3 toWayPoint = wayPoint - MonsterBTTask::mTransform->GetPosition().xz();

	if (toWayPoint.Length() > kMinDistance) {
		MonsterBTTask::mTransform->RotateTargetAxisY(wayPoint, mStat->GetStat_RotationSpeed());
		MonsterBTTask::mTransform->Translate(MonsterBTTask::mTransform->GetLook(), mPatrolSpeed * GetOwner()->GetDeltaTime());
	}
	else {
		mCurrWayPointIdx = (mCurrWayPointIdx + 1) % mWayPoints.size();
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_PATROL);
	return BTNodeState::Running;

}

MonsterTask::Patrol::Patrol(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_Patrol, callback)
{
	mPatrolSpeed     = mStat->GetStat_MoveSpeed() * 0.5f;
}

MonsterTask::Patrol::~Patrol()
{

}

void MonsterTask::Patrol::SetWayPoints(std::vector<Vec3>&& wayPoints)
{
	mWayPoints = wayPoints;
}
