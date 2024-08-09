#include "pch.h"
#include "BTTaskM_Attack.h"

#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{
	// ※ Attack Range BTNode 에서 Target 체크하므로 여기서는 할 필요 없음 
	SPtr<GameObject> target = mEnemyController->GetTarget();
	
	auto transSNS           = target->GetTransform()->GetSnapShot(); /* Lock */
	Vec3 TargetPos          = transSNS.GetPosition(); 
	
	// 1. Target Player 를 향해 회전한다.
	MonsterBTTask::mTransform->RotateTargetAxisY(TargetPos, 500.f);
	mStat->UpdatePrevHP();
	ExecuteCallback();

	return BTNodeState::Success;
}


MonsterTask::Attack::Attack(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_Attack, callback)
{
}


MonsterTask::Attack::~Attack()
{

}

