#pragma once

#include "BTTask.h"
#include "BTTaskM_PathPlanning_AStar.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Path Planning To Target 
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class PathPlanningToTarget : public PathPlanning_AStar {
	private:
	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanningToTarget(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~PathPlanningToTarget();
	};
};

