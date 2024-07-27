#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

namespace MonsterTask {
	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º TAsk Patrol 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Patrol : public MonsterBTTask {
	private:

	private:
		std::vector<Vec3>	mWayPoints{};
		int					mCurrWayPointIdx{};
		float				mPatrolSpeed{};

	public:
		virtual BTNodeState Evaluate() override;
		void SetWayPoints(std::vector<Vec3>&& wayPoints);

	public:
		Patrol(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~Patrol();
	};

};



