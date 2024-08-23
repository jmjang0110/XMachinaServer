#pragma once

#include "BTTask.h"
namespace MonsterTask {
	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Death 
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckDeath : public MonsterBTTask {
	private:
		float	mAccTime    = {};
		float	mRemoveTime = {};
		bool	mIsDead     = {};

	public:
		virtual BTNodeState Evaluate() override;
		void ExecuteCallback_CheckDeath();

	public:
		CheckDeath(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckDeath();
	};

};

