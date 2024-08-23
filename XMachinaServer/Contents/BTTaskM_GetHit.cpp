#include "pch.h"
#include "BTTaskM_GetHit.h"

#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Get Hit  
/// __________________________________________________________________________

BTNodeState MonsterTask::GetHit::Evaluate()
{
	//LOG_MGR->Cout("GetHit \n");

	if (mStat->GetStat_GetHitAnimName() == "None") {
		return BTNodeState::Failure;
	}

	const float crntHP = mStat->GetCrntHp();
	if (!mStat->UpdatePrevHP()) {
		mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_GETHIT);
		MonsterBTTask::mAnimation->GetController()->SetValue("GetHit", true);

		//Vec3 moveDir = mEnemyController->GetTarget()->GetTransform()->GetLook();
		//MonsterBTTask::mTransform->Translate(moveDir, mKnockBack);
		//mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_GETHIT);
	}

	if (MonsterBTTask::mAnimation->GetController()->GetParam("GetHit")->val.b == false)
		return BTNodeState::Failure;

	return BTNodeState::Success;
}


MonsterTask::GetHit::GetHit(SPtr<GameObject> owner, std::function<void()> callback)
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
