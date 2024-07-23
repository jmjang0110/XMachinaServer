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
	//LOG_MGR->Cout("GetHit \n");

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
		GetOwner()->GetAnimation()->GetController()->SetValue("GetHit", true);

		Vec3 TargetLook{};
		if (mEnemyController->IsMindControlled() == false)
			TargetLook = mEnemyController->GetTargetPlayer()->GetTransform()->GetSnapShot().GetLook();
		else
			TargetLook = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetLook();

		GetOwner()->GetTransform()->Translate(TargetLook, mKnockBack);
	}

	if (GetOwner()->GetAnimation()->GetController()->GetParam("GetHit")->val.b == false)
		return BTNodeState::Failure;

	return BTNodeState::Success;
}


MonsterTask::GetHit::GetHit(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_GetHit, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

	
	mPrevHp          = mStat->GetCrntHp();
	mKnockBack       = 0.05f;
	const auto& motion = GetOwner()->GetAnimation()->GetController()->FindMotionByName(mStat->GetStat_GetHitAnimName());
	motion->AddEndCallback(std::bind(&GetHit::GetHitEndCallback, this));

}

MonsterTask::GetHit::~GetHit()
{
	mEnemyController = nullptr;

}

void MonsterTask::GetHit::GetHitEndCallback()
{
	GetOwner()->GetAnimation()->GetController()->SetValue("GetHit", false);

}
