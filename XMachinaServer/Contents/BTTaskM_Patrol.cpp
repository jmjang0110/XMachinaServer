#include "pch.h"
#include "BTTask.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Patrol 
/// __________________________________________________________________________

BTNodeState MonsterTask::Patrol::Evaluate()
{
	LOG_MGR->Cout("Patrol \n");

	return BTNodeState();
}

MonsterTask::Patrol::Patrol(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Patrol, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::Patrol::~Patrol()
{
	mEnemyController = nullptr;

}