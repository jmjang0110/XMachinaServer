#pragma once

#include "BTTask.h"
namespace MonsterTask {
	class MoveToMindControlInvoker : public MonsterBTTask {
	private:
		SPtr<GameObject> mInvoker;

	public:
		virtual BTNodeState Evaluate() override;
		// x������ �������� ������� Ȯ���ϴ� �Լ� ����
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);

	public:
		MoveToMindControlInvoker(SPtr<GameObject> owner, std::function<void()> callback = nullptr);
		~MoveToMindControlInvoker();
	};

};


