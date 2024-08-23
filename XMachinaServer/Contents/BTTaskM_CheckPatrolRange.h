#pragma once

#include "BTTask.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Patrol Range
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckPatrolRange : public MonsterBTTask {
	private:

		Vec3  mSpawnPos    = {};
		Vec3  mBaryCenter  = {};
		float mPatrolRange = {};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckPatrolRange(SPtr<GameObject> owner, const Vec3& baryCenter, float patrolRange);
		CheckPatrolRange(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckPatrolRange();
	};
};

