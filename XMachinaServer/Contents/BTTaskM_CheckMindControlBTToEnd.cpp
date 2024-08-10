#include "pch.h"
#include "BTTaskM_CheckMindControlBTToEnd.h"
#include "BTTask.h"

BTNodeState MonsterTask::CheckMindControlBTToEnd::Evaluate()
{
	bool IsMindControlled = mRoot->GetEnemyController()->GetOwnerMonster()->GetIsMindControlled();

	if (IsMindControlled) {
		return BTNodeState::Success;
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckMindControlBTToEnd::CheckMindControlBTToEnd(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckMindControlBT, callback)
{
}

MonsterTask::CheckMindControlBTToEnd::~CheckMindControlBTToEnd()

{
}
