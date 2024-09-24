#include "pch.h"
#include "BTTaskM_LightBipedAttack.h"
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
///	> ¢º¢º¢º Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::LightBipedAttack::Evaluate()
{
	mStat->UpdatePrevHP();

	mAccTime += mOwner->DeltaTime();

	if (mAccTime >= mStat->GetStat_AttackCoolTime()) {
		mStat->StartAttack();
		mAccTime = 0.f;
	}

	return BTNodeState::Success;
}


MonsterTask::LightBipedAttack::LightBipedAttack(SPtr<GameObject> owner)
	: MonsterBTTask(owner, BTTaskType::MonT_LightBipedAttack)
{
}


MonsterTask::LightBipedAttack::~LightBipedAttack()
{

}

