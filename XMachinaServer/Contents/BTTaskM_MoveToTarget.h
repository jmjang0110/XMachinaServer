#pragma once
#include "BTTask.h"
namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Move To Target
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToTarget : public MonsterBTTask {
	private:
		int		Cnt  = 0;
		double	time = 0.0;

	public:
		virtual BTNodeState Evaluate() override;
		// x절편이 음수인지 양수인지 확인하는 함수 정의
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);


	public:
		MoveToTarget(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~MoveToTarget();
	};
};

