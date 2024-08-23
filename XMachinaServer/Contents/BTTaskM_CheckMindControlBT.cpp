#include "pch.h"
#include "BTTaskM_CheckMindControlBT.h"
#include "BTTask.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "FBsPacketFactory.h"
#include "RoomManager.h"
#include "GameRoom.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"

BTNodeState MonsterTask::CheckMindControlBT::Evaluate()
{
	bool IsMindControlled = mEnemyController->IsMindControlled();

	if (IsMindControlled) {
		if (mPrevMindControlled != IsMindControlled) {
			mEnemyController->Reset();
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_CHANGE_BT);
		}

		mPrevMindControlled = IsMindControlled;

		mMindControlledTime += mOwner->DeltaTime();

		if (mMindControlledTime >= mkMaxMindControlledTime) {
			auto spkt = FBS_FACTORY->SPkt_PlayerOnSkill(mEnemyController->GetInvoker()->GetID(), FBProtocol::PLAYER_SKILL_TYPE_MIND_CONTROL, 0, mOwner->GetID());
			ROOM_MGR->BroadcastRoom(mEnemyController->GetOwnerRoom()->GetID(), spkt);

			mMindControlledTime = 0.f;
			mEnemyController->OffMindControl();
			mEnemyController->SetInvoker(nullptr);
			mEnemyController->SetTarget(nullptr);

			return BTNodeState::Failure;
		}

		return BTNodeState::Success;
	}

	mPrevMindControlled = IsMindControlled;
	return BTNodeState::Failure;
}

MonsterTask::CheckMindControlBT::CheckMindControlBT(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckMindControlBT, callback)
{
}

MonsterTask::CheckMindControlBT::~CheckMindControlBT()

{
}
