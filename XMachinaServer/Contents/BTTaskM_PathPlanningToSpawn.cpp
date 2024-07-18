#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"


/// +-------------------------------------------------------------------------
///	> ������ Task Path Planning To Spawn 
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToSapwn::Evaluate()
{


	// ��ΰ� ����ٸ� ��� �� Ž��
	if (mEnemyController->GetPaths()->empty()) {

		// ���� ������ ������ ��ġ ���� Ÿ�� ���� �ε����� ��ȯ
		Path::Pos start = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(GetOwner()->GetTransform()->GetPosition());
		Path::Pos dest = RESOURCE_MGR->GetTileMap()->GetTileUniqueIndexFromPos(mSpawnPos);

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
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::PathPlanningToSapwn::~PathPlanningToSapwn()
{
	mEnemyController = nullptr;

}
