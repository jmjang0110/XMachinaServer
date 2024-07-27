#pragma once
#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Attack 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Attack : public MonsterBTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		Attack(SPtr_GameObject owner, std::function<void()> callback);
		~Attack();
	};
};
