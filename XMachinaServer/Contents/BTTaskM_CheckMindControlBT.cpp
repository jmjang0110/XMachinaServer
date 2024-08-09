#include "pch.h"
#include "BTTaskM_CheckMindControlBT.h"
#include "BTTask.h"

BTNodeState MonsterTask::CheckMindControlBT::Evaluate()
{
	bool IsMindControlled = mRoot->GetEnemyController()->GetOwnerMonster()->GetIsMindControlled();

	if (IsMindControlled) {
		if (mPrevMindControlled != IsMindControlled) {
			mEnemyController->Reset();
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_CHANGE_BT);
		}

		mPrevMindControlled = IsMindControlled;
		return BTNodeState::Success;
	}

	mPrevMindControlled = IsMindControlled;
	return BTNodeState::Failure;
}

MonsterTask::CheckMindControlBT::CheckMindControlBT(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckMindControlBT, callback)
{
}

MonsterTask::CheckMindControlBT::~CheckMindControlBT()

{
}
