#include "pch.h"
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

	/* Attack Range BTNode 에서 Target 체크하므로 여기서는 할 필요 없음 */
	LOG_MGR->Cout("[", mEnemyController->GetOwner()->GetName(), "] : Attack \n");

	bool IsMindControlled = mEnemyController->IsMindControlled();
	if (IsMindControlled == true)
	{
		/* Mind Control On */
		Vec3 TargetPos = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetPosition();
		GetOwner()->GetTransform()->RotateTargetAxisY(TargetPos, mStat->GetStat_AttackRotationSpeed());
		mStat->UpdatePrevHP();
		ExecuteCallback();

	}
	else 
	{
		/* Mind Control Off */

		Vec3 TargetPos = mEnemyController->GetTargetPlayer()->GetPosition();
		GetOwner()->GetTransform()->RotateTargetAxisY(TargetPos, mStat->GetStat_AttackRotationSpeed());
		mStat->UpdatePrevHP();
		ExecuteCallback();
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK);
	return BTNodeState::Success;
}


MonsterTask::Attack::Attack(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Attack, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

}


MonsterTask::Attack::~Attack()
{
	mEnemyController = nullptr;

}

