#include "pch.h"
#include "BTTaskM_GetHit.h"

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


	if (!mEnemyController->GetTarget())
		return  BTNodeState::Failure;

	const float crntHP = mStat->GetCrntHp();
	if (!mStat->UpdatePrevHP()) {
		mEnemyController->SetState(EnemyInfo::State::GetHit);
		GetOwner()->GetAnimation()->GetController()->SetValue("GetHit", true);

		Vec3 moveDir = mEnemyController->GetTarget()->GetTransform()->GetLook();
		GetOwner()->GetTransform()->Translate(moveDir, mKnockBack);
	}

	if (GetOwner()->GetAnimation()->GetController()->GetParam("GetHit")->val.b == false)
		return BTNodeState::Failure;

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_GETHIT);
	mEnemyController->GetOwnerMonster()->SetBTState(FBProtocol::MONSTER_BT_TYPE_GETHIT);

	return BTNodeState::Success;
}


MonsterTask::GetHit::GetHit(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_GetHit, callback)
{
	mPrevHp          = mStat->GetCrntHp();
	mKnockBack       = 0.05f;
	const auto& motion = GetOwner()->GetAnimation()->GetController()->FindMotionByName(mStat->GetStat_GetHitAnimName());

	if (motion) {
		motion->AddEndCallback(std::bind(&GetHit::GetHitEndCallback, this));
	}
}

MonsterTask::GetHit::~GetHit()
{
	mEnemyController = nullptr;

}

void MonsterTask::GetHit::GetHitEndCallback()
{
	GetOwner()->GetAnimation()->GetController()->SetValue("GetHit", false);
	mEnemyController->SetState(EnemyInfo::State::Idle);

}
