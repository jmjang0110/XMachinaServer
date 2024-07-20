#include "pch.h"
#include "BTTask.h"



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
	mStat            = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);
	mPrevHp          = mStat->GetCrntHp();
	mKnockBack       = 0.05f;
}

MonsterTask::GetHit::~GetHit()
{
	mEnemyController = nullptr;

}
