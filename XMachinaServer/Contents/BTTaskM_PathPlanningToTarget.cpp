#include "pch.h"
#include "BTTaskM_PathPlanningToTarget.h"

#include "BTTask.h"
#include "ResourceManager.h"
#include "TileMap.h"
#include "GameObject.h"
#include "Transform.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Planning To Target  
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToTarget::Evaluate()
{
	if (!mEnemyController->GetPathTargetObject()) {
		return BTNodeState::Failure;
	}

	// 경로가 비었다면 경로 재 탐색
	if (MonsterTask::PathPlanning_AStar::mPath->empty()) {
		GetOwner()->GetAnimation()->GetController()->SetValue("Return", false);

		// 시작 지점과 목적지 위치 값을 타일 고유 인덱스로 변환
		
		SPtr<TileMap> tileMap = RESOURCE_MGR->GetTileMap();

		Vec3 d = mEnemyController->GetPathTargetObject()->GetTransform()->GetSnapShot().GetPosition();

		Path::Pos start = tileMap->GetTileUniqueIndexFromPos(GetOwner()->GetTransform()->GetPosition());
		Path::Pos dest  = tileMap->GetTileUniqueIndexFromPos(d);

		// 경로 계획에 실패했다면 Failure를 호출하여 다음 노드로 넘어감
		if (PathPlanningAStar(start, dest)) {
			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}

MonsterTask::PathPlanningToTarget::PathPlanningToTarget(SPtr_GameObject owner, std::function<void()> callback)
	: PathPlanning_AStar(owner, BTTaskType::MonT_PathPlanningToTarget, callback)
{

	MonsterTask::PathPlanning_AStar::mPath = mEnemyController->GetPaths();

}

MonsterTask::PathPlanningToTarget::~PathPlanningToTarget()
{
	mEnemyController = nullptr;

}