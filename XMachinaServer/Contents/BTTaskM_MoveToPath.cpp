#include "pch.h"
#include "BTTaskM_MoveToPath.h"
#include "BTTask.h"

#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"


#include "Script_Enemy.h"
#include "Script_EnemyController.h"


/// +-------------------------------------------------------------------------
///	> ������ Task Move To Path  
/// __________________________________________________________________________

MonsterTask::MoveToPath::MoveToPath(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_MoveToPath, callback)
{

	mMoveSpeed   = mStat->GetStat_MoveSpeed();
	mReturnSpeed = 1.7f * mMoveSpeed;
	mPaths       = mEnemyController->GetPaths();
	mReturnParam = mOwner->GetAnimation()->GetController()->GetParam("Return");
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
	if (mPaths->empty())
		return BTNodeState::Failure;

	MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);

	// ���� ��α����� ����
	Vec3 pos = mOwner->GetTransform()->GetPosition();
	Vec3 nextPos = (mPaths->top() - pos).xz();

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
	MonsterBTTask::mTransform->RotateTargetAxisY(mPaths->top(), mStat->GetStat_RotationSpeed());
	MonsterBTTask::mTransform->Translate(XMVector3Normalize(nextPos), speed * mOwner->DeltaTime());

	// ���� ��ο� ���� �� �ش� ��� ����
	const float kMinDistance = 0.1f;
	if (nextPos.Length() < kMinDistance) {
		mPaths->pop();
	}


	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_PATH);

	return BTNodeState::Success;
}
