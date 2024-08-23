#include "pch.h"
#include "BTTaskM_CheckMindControlBTToEnd.h"
#include "BTTask.h"
#include "BTNode.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"



BTNodeState MonsterTask::CheckMindControlBTToEnd::Evaluate()
{
	bool IsMindControlled = mEnemyController->IsMindControlled();
	if (IsMindControlled) {
		return BTNodeState::Success;
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckMindControlBTToEnd::CheckMindControlBTToEnd(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckMindControlBT, callback)
{
}

MonsterTask::CheckMindControlBTToEnd::~CheckMindControlBTToEnd()

{
}
