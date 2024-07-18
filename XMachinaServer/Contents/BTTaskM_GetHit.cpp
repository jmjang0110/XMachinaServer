#include "pch.h"
#include "BTTask.h"



/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Get Hit  
/// __________________________________________________________________________

BTNodeState MonsterTask::GetHit::Evaluate()
{
	LOG_MGR->Cout("GetHit \n");

	if (!mEnemyController->GetTargetObject()) {
		return BTNodeState::Failure;
	}

	const float crntHp = mStat->GetCrntHp();
	if (!mStat->UpdatePrevHP()) {
		mEnemyController->SetState(EnemyInfo::State::GetHit);

		Vec3 TargetLook = mEnemyController->GetTargetObject()->GetTransform()->GetLook();
		GetOwner()->GetTransform()->Translate(TargetLook, mKnockBack);
	}

	return BTNodeState::Success;
}


MonsterTask::GetHit::GetHit(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_GetHit, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);
	mPrevHp = mStat->GetCrntHp();
	mKnockBack = 0.05f;
}

MonsterTask::GetHit::~GetHit()
{
	mEnemyController = nullptr;

}
