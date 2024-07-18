#include "pch.h"
#include "BTTask.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Planning To Target  
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToTarget::Evaluate()
{
	return BTNodeState();
}

MonsterTask::PathPlanningToTarget::PathPlanningToTarget(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_PathPlanningToTarget, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::PathPlanningToTarget::~PathPlanningToTarget()
{
	mEnemyController = nullptr;

}