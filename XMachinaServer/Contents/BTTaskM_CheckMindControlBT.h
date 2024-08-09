#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"


namespace MonsterTask {
	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Mind Control BT
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckMindControlBT : public MonsterBTTask
	{
	private:
		bool mPrevMindControlled = false;
		bool mIsMindControlled = false; 
	public:
		virtual BTNodeState Evaluate() override;
	public:
		CheckMindControlBT(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckMindControlBT();
	};


};

