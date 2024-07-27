#pragma once
#include "BTTask.h"
#include "BTTaskM_PathPlanning_AStar.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

namespace MonsterTask {

	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Path Planning To Spawn  
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class PathPlanningToSapwn : public PathPlanning_AStar {
	private:
		Vec3						  mSpawnPos{};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanningToSapwn(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~PathPlanningToSapwn();
	};
};

