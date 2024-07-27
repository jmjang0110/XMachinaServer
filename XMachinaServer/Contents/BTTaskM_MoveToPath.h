#pragma once

#include "BTTask.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ������ Task Move To Path
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToPath : public MonsterBTTask {
	private:
		float						  mMoveSpeed{};
		float						  mReturnSpeed{};
		std::stack<Vec3>* mPath;

		const AnimatorParameter* mReturnParam{};
	public:
		virtual BTNodeState Evaluate() override;
		// x������ �������� ������� Ȯ���ϴ� �Լ� ����
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);
	public:
		MoveToPath(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~MoveToPath();
	};

};

