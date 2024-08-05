#include "pch.h"
#include "BTTaskM_PathPlanningToSpawn.h"

#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"

/// +-------------------------------------------------------------------------
///	> ������ Task Path Planning To Spawn 
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToSapwn::Evaluate()
{
	// ��ΰ� ����ٸ� ��� �� Ž��
	if (mEnemyController->GetPaths()->empty()) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Return", true);


		// ���� ������ ������ ��ġ ���� Ÿ�� ���� �ε����� ��ȯ
		Path::Pos start = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(GetOwner()->GetTransform()->GetPosition());
		Path::Pos dest  = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(mSpawnPos);

		// ��� ��ȹ�� �����ߴٸ� Failure�� ȣ���Ͽ� ���� ���� �Ѿ b
		if (MonsterTask::PathPlanning_AStar::PathPlanningAStar(start, dest)) {
			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}


MonsterTask::PathPlanningToSapwn::PathPlanningToSapwn(SPtr_GameObject owner, std::function<void()> callback)
	: PathPlanning_AStar(owner, BTTaskType::MonT_PathPlanningToSpawn, callback)
{
	mSpawnPos = owner->GetTransform()->GetPosition();

}

MonsterTask::PathPlanningToSapwn::~PathPlanningToSapwn()
{

}
