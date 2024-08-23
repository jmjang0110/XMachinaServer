#pragma once

#include "BTTask.h"
namespace MonsterTask {
	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Mind Control BT
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckMindControlBT : public MonsterBTTask
	{
	private:
		static constexpr float mkMaxMindControlledTime = 30.f;

		bool	mPrevMindControlled = false;
		bool	mIsMindControlled   = false; 
		float	mMindControlledTime = {};

	public:
		virtual BTNodeState Evaluate() override;
	public:
		CheckMindControlBT(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckMindControlBT();
	};


};

