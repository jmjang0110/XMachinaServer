#pragma once
#include "BTTask.h"
#include "AStarPath.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

namespace MonsterTask {
	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Path Planning A*
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class PathPlanning_AStar : public MonsterBTTask {
	private:

	protected:
		std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

		std::stack<Vec3>* mPath;
		Path::Pos						mStart;
		Path::Pos						mDest;

		int								mkWeight = 10;
		int								mkMaxVisited = 2000;

	public:
		virtual BTNodeState Evaluate() override;
		bool PathPlanningAStar(Path::Pos start, Path::Pos dest);
		Path::Pos FindNoneTileFromBfs(const Path::Pos& pos);

	public:
		PathPlanning_AStar(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback = nullptr);
		PathPlanning_AStar(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~PathPlanning_AStar();
	};

};

