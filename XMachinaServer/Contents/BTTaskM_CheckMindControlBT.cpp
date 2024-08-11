#include "pch.h"
#include "BTTaskM_CheckMindControlBT.h"
#include "BTTask.h"

#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "GameRoom.h"

BTNodeState MonsterTask::CheckMindControlBT::Evaluate()
{
	bool IsMindControlled = mRoot->GetEnemyController()->GetOwnerMonster()->GetIsMindControlled();

	if (IsMindControlled) {
		if (mPrevMindControlled != IsMindControlled) {
			mEnemyController->Reset();
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_CHANGE_BT);
		}

		mPrevMindControlled = IsMindControlled;

		mMindControlledTime += GetOwner()->GetDeltaTime();

		if (mMindControlledTime >= mkMaxMindControlledTime) {
			auto spkt = FBS_FACTORY->SPkt_PlayerOnSkill(mRoot->GetEnemyController()->GetInvoker()->GetID(), FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL, 0, GetOwner()->GetID());
			GAME_MGR->BroadcastRoom(mEnemyController->GetOwnerRoom()->GetID(), spkt);

			mMindControlledTime = 0.f;
			mRoot->GetEnemyController()->GetOwnerMonster()->SetMindControlled(false);
			mRoot->GetEnemyController()->SetInvoker(nullptr);
			mEnemyController->SetTarget(nullptr);

			return BTNodeState::Failure;
		}

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
