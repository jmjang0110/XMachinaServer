#include "pch.h"
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

		// ���� ������ ������ ��ġ ���� Ÿ�� ���� �ε����� ��ȯ
		
		SPtr<TileMap> tileMap = RESOURCE_MGR->GetTileMap();

		Vec3 d = mEnemyController->GetPathTargetObject()->GetTransform()->GetSnapShot().GetPosition();

		Path::Pos start = tileMap->GetTileUniqueIndexFromPos(GetOwner()->GetTransform()->GetPosition());
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
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

	MonsterTask::PathPlanning_AStar::mPath = mEnemyController->GetPaths();

}

MonsterTask::PathPlanningToTarget::~PathPlanningToTarget()
{
	mEnemyController = nullptr;

}