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

	if (mEnemyController->GetState() == EnemyInfo::State::Attack) {
		//std::cout << "ATTACKING" << std::endl;
	}

	if (!mStat->IsDead())
		return BTNodeState::Failure;

	mEnemyController->SetState(EnemyInfo::State::Death);
	GetOwner()->GetAnimation()->GetController()->SetValue("Death", true);

	mAccTime += DELTA_TIME;

	ExecuteCallback();

	if (mAccTime >= mRemoveTime) {
		GetOwner()->DeActivate();
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_DEATH);
	return BTNodeState::Success;
}

MonsterTask::CheckDeath::CheckDeath(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDeath, callback)

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
