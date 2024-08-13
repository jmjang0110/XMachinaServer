#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

namespace MonsterTask {
	class CheckAttackRange : public MonsterBTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckAttackRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckAttackRange();
	};
};
