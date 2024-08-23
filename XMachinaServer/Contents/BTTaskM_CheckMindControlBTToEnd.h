#pragma once

#include "BTTask.h"
namespace MonsterTask {
	/// +-------------------------------------------------------------------------
	///	> ������ Task Check Mind Control BT
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckMindControlBTToEnd : public MonsterBTTask
	{
	public:
		virtual BTNodeState Evaluate() override;
	public:
		CheckMindControlBTToEnd(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckMindControlBTToEnd();
	};


};

