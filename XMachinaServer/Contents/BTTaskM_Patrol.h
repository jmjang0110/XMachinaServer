#pragma once

#include "BTTask.h"
namespace MonsterTask {
	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º TAsk Patrol 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Patrol : public MonsterBTTask {
	private:

	private:
		std::vector<Vec3>	mWayPoints       = {};
		int					mCurrWayPointIdx = {};
		float				mPatrolSpeed     = {};

	public:
		virtual BTNodeState Evaluate() override;
		void SetWayPoints(std::vector<Vec3>&& wayPoints);

	public:
		Patrol(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~Patrol();
	};

};



