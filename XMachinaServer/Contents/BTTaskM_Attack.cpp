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
///	> ¢º¢º¢º Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{
	LOG_MGR->Cout("Attack \n");

	if (mEnemyController->IsMindControlled())
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
	return BTNodeState::Success;
}


MonsterTask::Attack::Attack(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Attack, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);

}

MonsterTask::Attack::~Attack()
{
	mEnemyController = nullptr;

}

