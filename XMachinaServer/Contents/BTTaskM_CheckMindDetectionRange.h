#pragma once

#include "BTTask.h"
namespace MonsterTask {
	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Mind Detection Range
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckMindDetectionRange : public MonsterBTTask {
	private:

	private:
		bool SetTargetNearestEnemy();
	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckMindDetectionRange(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckMindDetectionRange();
	};

};
