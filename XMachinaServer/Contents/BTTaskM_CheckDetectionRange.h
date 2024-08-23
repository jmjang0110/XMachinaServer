#pragma once

#include "BTTask.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Detection Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckDetectionRange : public MonsterBTTask {
	private:
		SPtr<GameObject>			mTargetPlayer;
		SPtr<GameObject>			mTargetMonster;

	public:
		virtual BTNodeState Evaluate() override;

		SPtr<GameObject> FindDetectionPlayer();
	public:
		CheckDetectionRange(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckDetectionRange();
	};

};

