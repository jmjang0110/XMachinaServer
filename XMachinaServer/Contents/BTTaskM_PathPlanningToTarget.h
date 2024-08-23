#pragma once

#include "BTTaskM_PathPlanning_AStar.h"
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
		PathPlanningToTarget(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~PathPlanningToTarget();
	};
};

