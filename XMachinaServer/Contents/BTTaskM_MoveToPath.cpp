#include "pch.h"
#include "BTTask.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
#include "Animation.h"

#include "ServerLib/ThreadManager.h"

/// +-------------------------------------------------------------------------
///	> ������ Task Move To Path  
/// __________________________________________________________________________

MonsterTask::MoveToPath::MoveToPath(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_MoveToPath, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

	mMoveSpeed = mStat->GetStat_MoveSpeed();
	mReturnSpeed = 1.7f * mMoveSpeed;
	mPath = mEnemyController->GetPaths();

	mReturnParam = GetOwner()->GetAnimation()->GetController()->GetParam("Return");
}

MonsterTask::MoveToPath::~MoveToPath()
{
	mEnemyController = nullptr;

}

bool MonsterTask::MoveToPath::isXInterceptPositive(const Vec3& To, const Vec3& From)
{
	// ������Ʈ�κ��� Ÿ�ٱ����� ����
	Vec3 toTarget = To - From;
	if (toTarget.x == 0) {
		return false;
	}

	double m = To.z / To.x;
	double c = From.z - m * From.x;

	// x������ y = 0�� ��, x = -c / m
	double xIntercept = -c / m;

	return xIntercept > 0;
}

BTNodeState MonsterTask::MoveToPath::Evaluate()
{
	if (mPath->empty())
		return BTNodeState::Failure;

	GetOwner()->GetAnimation()->GetController()->SetValue("Walk", true);

	// ���� ��α����� ����
	Vec3 a = GetOwner()->GetTransform()->GetPosition();
	Vec3 nextPos = (mPath->top() - a).xz();

	// ���� ���� ���¶�� ���ǵ带 �ø���.
	float speed{};
	if (mReturnParam->val.b) {
		speed = mReturnSpeed;
		//mWalkMotion->SetSpeed(1.7f);
	}
	else {
		speed = mMoveSpeed;
		//mWalkMotion->SetSpeed(1.f);
	}

	// ���� ��θ� ���� �̵� �� ȸ��
	GetOwner()->GetTransform()->RotateTargetAxisY(mPath->top(), mStat->GetStat_RotationSpeed());
	GetOwner()->GetTransform()->Translate(XMVector3Normalize(nextPos), speed * DELTA_TIME);

	Vec3 pos = GetOwner()->GetTransform()->GetPosition();
	mEnemyController->GetOwnerMonster()->SetPosition(pos); /* Snap Shot - Pos */

	LOG_MGR->Cout("[ ", GetOwner()->GetID(), " ] : ", pos.x, " ", pos.y, " ", pos.z, "\n");

	// ���� ��ο� ���� �� �ش� ��� ����
	const float kMinDistance = 0.1f;
	if (nextPos.Length() < kMinDistance)
		mPath->pop();

	return BTNodeState::Success;
}
