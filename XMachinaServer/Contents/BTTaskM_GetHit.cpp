#include "pch.h"
#include "BTTask.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"


/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Get Hit  
/// __________________________________________________________________________

BTNodeState MonsterTask::GetHit::Evaluate()
{
	LOG_MGR->Cout("GetHit \n");

	if (mEnemyController->IsMindControlled() == false) {
		if (!mEnemyController->GetTargetPlayer())
			return BTNodeState::Failure;
	}
	else {
		if (!mEnemyController->GetTargetMonster())
			return BTNodeState::Failure;
	}

	const float crntHp = mStat->GetCrntHp();
	if (!mStat->UpdatePrevHP()) {
		mEnemyController->SetState(EnemyInfo::State::GetHit);

		Vec3 TargetLook{};
		if (mEnemyController->IsMindControlled() == false)
			TargetLook = mEnemyController->GetTargetPlayer()->GetTransform()->GetSnapShot().GetLook();
		else
			TargetLook = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetLook();

		GetOwner()->GetTransform()->Translate(TargetLook, mKnockBack);
	}

	return BTNodeState::Success;
}


MonsterTask::GetHit::GetHit(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_GetHit, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);	
	
	mPrevHp          = mStat->GetCrntHp();
	mKnockBack       = 0.05f;
}

MonsterTask::GetHit::~GetHit()
{
	mEnemyController = nullptr;

}
