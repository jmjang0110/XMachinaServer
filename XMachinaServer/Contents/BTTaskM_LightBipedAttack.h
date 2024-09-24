#pragma once
#include "BTTask.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ������ Task Attack 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class LightBipedAttack : public MonsterBTTask {
	private:
		float mAccTime{};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		LightBipedAttack(SPtr<GameObject> owner);
		~LightBipedAttack();
	};
};
