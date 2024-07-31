#include "pch.h"
#include "BTTaskM_CheckDeath.h"

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
	if (!mStat->IsDead())
		return BTNodeState::Failure;

	mEnemyController->SetState(EnemyInfo::State::Death);

	mAccTime += GetOwner()->GetDeltaTime();

	mEnemyController->RemoveAllAnimation();
	GetOwner()->GetAnimation()->GetController()->SetValue("Death", true);
	
	ExecuteCallback();

	if (mAccTime >= mRemoveTime) {
		GetOwner()->DeActivate();
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_DEATH);
	
	return BTNodeState::Success;
}

MonsterTask::CheckDeath::CheckDeath(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckDeath, callback)

{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

}

MonsterTask::CheckDeath::~CheckDeath()
{
	mEnemyController = nullptr;

}

void MonsterTask::CheckDeath::ExecuteCallback_CheckDeath()
{
}
