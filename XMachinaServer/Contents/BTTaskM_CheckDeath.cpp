#include "pch.h"
#include "BTTask.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Death  
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDeath::Evaluate()
{
	LOG_MGR->Cout("CheckDeath\n");

	if (!mStat->IsDead())
		return BTNodeState::Failure;

	mEnemyController->SetState(EnemyInfo::State::Death);


	mAccTime += DELTA_TIME;

	ExecuteCallback();

	if (mAccTime >= mRemoveTime) {
		GetOwner()->DeActivate();
	}

	return BTNodeState::Success;
}

MonsterTask::CheckDeath::CheckDeath(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDeath, callback)

{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckDeath::~CheckDeath()
{
	mEnemyController = nullptr;

}