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
///	> ������ Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{
	// �� Attack Range BTNode ���� Target üũ�ϹǷ� ���⼭�� �� �ʿ� ���� 
	
	// 1. Target Player �� ���� ȸ���Ѵ�.
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

