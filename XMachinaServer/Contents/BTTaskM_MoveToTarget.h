#pragma once
#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ������ Task Move To Target
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToTarget : public MonsterBTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;
		// x������ �������� ������� Ȯ���ϴ� �Լ� ����
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);


	public:
		MoveToTarget(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~MoveToTarget();
	};
};

