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
///	> ▶▶▶ Task Path Planning To Spawn 
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToSapwn::Evaluate()
{
	// 경로가 비었다면 경로 재 탐색
	if (mEnemyController->GetPaths()->empty()) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Return", true);


		// 시작 지점과 목적지 위치 값을 타일 고유 인덱스로 변환
		Path::Pos start = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(mOwner->GetTransform()->GetPosition());
		Path::Pos dest  = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(mSpawnPos);

		// 경로 계획에 실패했다면 Failure를 호출하여 다음 노드로 넘어감 b
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
