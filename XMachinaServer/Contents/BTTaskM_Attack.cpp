#include "pch.h"
#include "BTTaskM_Attack.h"
#include "BTTask.h"
#include "BTNode.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_Player.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"


/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{
	// ※ Attack Range BTNode 에서 Target 체크하므로 여기서는 할 필요 없음 
	
	// 1. Target Player 를 향해 회전한다.
	//MonsterBTTask::mTransform->RotateTargetAxisY(TargetPos, 500.f);
	mStat->UpdatePrevHP();
	ExecuteCallback();

	return BTNodeState::Success;
}


MonsterTask::Attack::Attack(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_Attack, callback)
{
}


MonsterTask::Attack::~Attack()
{

}

