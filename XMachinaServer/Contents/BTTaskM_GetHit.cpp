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
		mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_GETHIT);
		MonsterBTTask::mAnimation->GetController()->SetValue("GetHit", true);

		Vec3 moveDir = mEnemyController->GetTarget()->GetTransform()->GetLook();
		MonsterBTTask::mTransform->Translate(moveDir, mKnockBack);
	}

	if (MonsterBTTask::mAnimation->GetController()->GetParam("GetHit")->val.b == false)
		return BTNodeState::Failure;

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_GETHIT);

	return BTNodeState::Success;
}


MonsterTask::GetHit::GetHit(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_GetHit, callback)
{
	mPrevHp          = mStat->GetCrntHp();
	mKnockBack       = 0.05f;
	const auto& motion = MonsterBTTask::mAnimation->GetController()->FindMotionByName(mStat->GetStat_GetHitAnimName());

	if (motion) {
		motion->AddEndCallback(std::bind(&GetHit::GetHitEndCallback, this));
	}
}

MonsterTask::GetHit::~GetHit()
{
}

void MonsterTask::GetHit::GetHitEndCallback()
{
	MonsterBTTask::mAnimation->GetController()->SetValue("GetHit", false);
	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
}
