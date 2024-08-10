#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
namespace MonsterTask {
	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Death 
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckDeath : public MonsterBTTask {
	private:
		float mAccTime{};
		float mRemoveTime{};
		bool mIsDead{};

	public:
		virtual BTNodeState Evaluate() override;
		void ExecuteCallback_CheckDeath();

	public:
		CheckDeath(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckDeath();
	};

};

