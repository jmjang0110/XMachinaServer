#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Patrol Range
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckPatrolRange : public MonsterBTTask {
	private:

		Vec3  mSpawnPos{};
		Vec3  mBaryCenter{};
		float mPatrolRange{};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckPatrolRange(SPtr_GameObject owner, const Vec3& baryCenter, float patrolRange);
		CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckPatrolRange();
	};
};

