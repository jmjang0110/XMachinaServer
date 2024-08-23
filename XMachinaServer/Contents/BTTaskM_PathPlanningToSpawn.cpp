#include "pch.h"
#include "BTTaskM_PathPlanningToSpawn.h"
#include "BTTask.h"
#include "BTNode.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "Script_Player.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

/// +-------------------------------------------------------------------------
///	> ������ Task Path Planning To Spawn 
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToSapwn::Evaluate()
{
	// ��ΰ� ����ٸ� ��� �� Ž��
	if (mEnemyController->GetPaths()->empty()) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Return", true);


		// ���� ������ ������ ��ġ ���� Ÿ�� ���� �ε����� ��ȯ
		Path::Pos start = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(mOwner->GetTransform()->GetPosition());
		Path::Pos dest  = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(mSpawnPos);

		// ��� ��ȹ�� �����ߴٸ� Failure�� ȣ���Ͽ� ���� ���� �Ѿ b
		if (MonsterTask::PathPlanning_AStar::PathPlanningAStar(start, dest)) {
			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}


MonsterTask::PathPlanningToSapwn::PathPlanningToSapwn(SPtr<GameObject> owner, std::function<void()> callback)
	: PathPlanning_AStar(owner, BTTaskType::MonT_PathPlanningToSpawn, callback)
{
	mSpawnPos = owner->GetTransform()->GetPosition();

}

MonsterTask::PathPlanningToSapwn::~PathPlanningToSapwn()
{

}
