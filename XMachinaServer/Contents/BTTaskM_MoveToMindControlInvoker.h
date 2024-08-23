#pragma once

#include "BTTask.h"
namespace MonsterTask {
	class MoveToMindControlInvoker : public MonsterBTTask {
	private:
		SPtr<GameObject> mInvoker;

	public:
		virtual BTNodeState Evaluate() override;
		// x절편이 음수인지 양수인지 확인하는 함수 정의
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);

	public:
		MoveToMindControlInvoker(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~MoveToMindControlInvoker();
	};

};


