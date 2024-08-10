#include "pch.h"
#include "BTTaskM_CheckDeath.h"

#include "BTTask.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
#include "ServerLib/ThreadManager.h"
#include "Script_PheroDropper.h"

#include "FBsPacketFactory.h"
#include "NPCController.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GameMonster.h"

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Death  
/// __________________________________________________________________________
 
BTNodeState MonsterTask::CheckDeath::Evaluate()
{
	if (auto target = mEnemyController->GetTarget()) {
		if (target->GetScript<Script_Stat>(ScriptInfo::Type::Stat)->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			mEnemyController->SetTarget(nullptr);
		}
	}

	if (mStat->S_GetObjectState() != Script_Stat::ObjectState::Dead) {
		return BTNodeState::Failure;
	}

	if (!mIsDead) {
		Vec3 pos = GetOwner()->GetTransform()->GetPosition();
		auto pheroDropper = GetOwner()->GetScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
		pheroDropper->SetPherosPos(pos);

		auto spkt = FBS_FACTORY->SPkt_DeadMonster(GetOwner()->GetID(), pos, mRoot->GetEnemyController()->GetOwnerMonster()->GetPheros());
		GAME_MGR->BroadcastRoom(mRoot->GetEnemyController()->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetID(), spkt);

		mIsDead = true;
	}

	mAccTime += GetOwner()->GetDeltaTime();

	mEnemyController->RemoveAllAnimation();
	MonsterBTTask::mAnimation->GetController()->SetValue("Death", true);
	
	ExecuteCallback();

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_DEATH);
	
	return BTNodeState::Success;
}

MonsterTask::CheckDeath::CheckDeath(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckDeath, callback)

{

}

MonsterTask::CheckDeath::~CheckDeath()
{
}

void MonsterTask::CheckDeath::ExecuteCallback_CheckDeath()
{
}
