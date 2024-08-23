#pragma once

#include "BTTask.h"
namespace MonsterTask {
	class CheckAttackRange : public MonsterBTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckAttackRange(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~CheckAttackRange();
	};
};
