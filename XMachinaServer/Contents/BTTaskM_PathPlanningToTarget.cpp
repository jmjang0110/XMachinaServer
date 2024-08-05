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
///	> ������ Task Planning To Target  
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToTarget::Evaluate()
{
	if (!mEnemyController->GetPathTargetObject()) {
		return BTNodeState::Failure;
	}

	// ��ΰ� ����ٸ� ��� �� Ž��
	if (MonsterTask::PathPlanning_AStar::mPath->empty()) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Return", false);

		// ���� ������ ������ ��ġ ���� Ÿ�� ���� �ε����� ��ȯ
		
		SPtr<TileMap> tileMap = RESOURCE_MGR->GetTileMap();

		Vec3 d = mEnemyController->GetPathTargetObject()->GetTransform()->GetSnapShot().GetPosition();

		Vec3 pos = MonsterBTTask::mTransform->GetPosition();
		Path::Pos start = tileMap->GetTileUniqueIndexFromPos(pos);
		Path::Pos dest  = tileMap->GetTileUniqueIndexFromPos(d);

		// ��� ��ȹ�� �����ߴٸ� Failure�� ȣ���Ͽ� ���� ���� �Ѿ
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