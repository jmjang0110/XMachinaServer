#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Detection Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckDetectionRange : public MonsterBTTask {
	private:
		SPtr<GamePlayer>			mTargetPlayer;
		SPtr<GameMonster>			mTargetMonster;

	public:
		virtual BTNodeState Evaluate() override;

		SPtr<GamePlayer> FindDetectionPlayer();


	public:
		CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckDetectionRange();
	};

};

