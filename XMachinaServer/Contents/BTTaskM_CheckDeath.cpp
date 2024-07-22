#include "pch.h"
#include "BTTask.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
#include "ServerLib/ThreadManager.h"

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Death  
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDeath::Evaluate()
{
	//LOG_MGR->Cout("CheckDeath\n");

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

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);


}

MonsterTask::CheckDeath::~CheckDeath()
{
	mEnemyController = nullptr;

}

void MonsterTask::CheckDeath::ExecuteCallback_CheckDeath()
{
}
