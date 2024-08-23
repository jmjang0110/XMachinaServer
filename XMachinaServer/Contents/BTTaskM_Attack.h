#pragma once
#include "BTTask.h"
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
		Attack(SPtr<GameObject> owner, std::function<void()> callback);
		~Attack();
	};
};
