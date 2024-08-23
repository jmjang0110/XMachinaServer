#pragma once

#include "BTTask.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Get Hit 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class GetHit : public MonsterBTTask {
	private:
		float mPrevHp    = {};
		float mKnockBack = {};

	public:
		virtual BTNodeState Evaluate() override;
		void GetHitEndCallback();


	public:
		GetHit(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~GetHit();
	};
};

