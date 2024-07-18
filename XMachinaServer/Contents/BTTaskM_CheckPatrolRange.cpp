#include "pch.h"
#include "BTTask.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Patrol Range 
/// __________________________________________________________________________


BTNodeState MonsterTask::CheckPatrolRange::Evaluate()
{
	LOG_MGR->Cout("CheckPatrolRange \n");

	return BTNodeState();
}

MonsterTask::CheckPatrolRange::CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckPatrolRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckPatrolRange::~CheckPatrolRange()
{
	mEnemyController = nullptr;

}