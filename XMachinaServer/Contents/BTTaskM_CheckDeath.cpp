#include "pch.h"
#include "BTTaskM_CheckDeath.h"
#include "BTTask.h"
#include "ServerLib/ThreadManager.h"
#include "Script_PheroDropper.h"


#include "Script_Enemy.h"
#include "Script_EnemyController.h"

#include "FBsPacketFactory.h"
#include "NPCController.h"
#include "RoomManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "Collider.h"


/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Death  
/// __________________________________________________________________________
 
BTNodeState MonsterTask::CheckDeath::Evaluate()
{
	if (auto target = mEnemyController->GetTarget()) {
		auto script_stat = target->GetScriptEntity<Script_Stat>();
		if (script_stat->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			mEnemyController->SetTarget(nullptr);
		}
	}

	if (mStat->S_GetObjectState() != Script_Stat::ObjectState::Dead) {
		return BTNodeState::Failure;
	}

	if (!mIsDead) {
		Vec3 pos = mOwner->GetTransform()->GetPosition();
		auto pheroDropper = mOwner->GetScript<Script_PheroDropper>();

		pheroDropper->SetPherosPos(pos);
		auto spkt = FBS_FACTORY->SPkt_DeadMonster(mOwner->GetID(), pos, pheroDropper->GetPherosString());
		ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), spkt);

		mIsDead = true;
	}

	mAccTime += mOwner->DeltaTime();

	mEnemyController->RemoveAllAnimation();
	MonsterBTTask::mAnimation->GetController()->SetValue("Death", true);
	
	ExecuteCallback();

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_DEATH);
	
	return BTNodeState::Success;
}

MonsterTask::CheckDeath::CheckDeath(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckDeath, callback)

{

}

MonsterTask::CheckDeath::~CheckDeath()
{
}

void MonsterTask::CheckDeath::ExecuteCallback_CheckDeath()
{
}
