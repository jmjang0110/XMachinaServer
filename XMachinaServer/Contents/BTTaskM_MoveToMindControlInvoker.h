#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
namespace MonsterTask {
	class BTTaskM_MoveToMindControlInvoker : public MonsterBTTask {
	private:
		SPtr<GamePlayer> mInvoker;

	public:
		virtual BTNodeState Evaluate() override;
		// x������ �������� ������� Ȯ���ϴ� �Լ� ����
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);


	public:
		BTTaskM_MoveToMindControlInvoker(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~BTTaskM_MoveToMindControlInvoker();
	};



};


