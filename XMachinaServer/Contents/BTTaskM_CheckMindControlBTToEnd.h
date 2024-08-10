#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"


namespace MonsterTask {
	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Mind Control BT
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckMindControlBTToEnd : public MonsterBTTask
	{
	public:
		virtual BTNodeState Evaluate() override;
	public:
		CheckMindControlBTToEnd(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckMindControlBTToEnd();
	};


};

