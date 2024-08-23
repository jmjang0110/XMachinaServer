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
///	> ▶▶▶ Task Move To Path  
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
	// 오브젝트로부터 타겟까지의 벡터
	Vec3 toTarget = To - From;
	if (toTarget.x == 0) {
		return false;
	}

	double m = To.z / To.x;
	double c = From.z - m * From.x;

	// x절편은 y = 0일 때, x = -c / m
	double xIntercept = -c / m;

	return xIntercept > 0;
}

BTNodeState MonsterTask::MoveToPath::Evaluate()
{
	if (mPaths->empty())
		return BTNodeState::Failure;

	MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);

	// 다음 경로까지의 벡터
	Vec3 pos = mOwner->GetTransform()->GetPosition();
	Vec3 nextPos = (mPaths->top() - pos).xz();

	// 현재 복귀 상태라면 스피드를 올린다.
	float speed{};
	if (mReturnParam->val.b) {
		speed = mReturnSpeed;
		//mWalkMotion->SetSpeed(1.7f);
	}
	else {
		speed = mMoveSpeed;
		//mWalkMotion->SetSpeed(1.f);
	}

	// 다음 경로를 향해 이동 및 회전
	MonsterBTTask::mTransform->RotateTargetAxisY(mPaths->top(), mStat->GetStat_RotationSpeed());
	MonsterBTTask::mTransform->Translate(XMVector3Normalize(nextPos), speed * mOwner->DeltaTime());

	// 다음 경로에 도착 시 해당 경로 삭제
	const float kMinDistance = 0.1f;
	if (nextPos.Length() < kMinDistance) {
		mPaths->pop();
	}


	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_PATH);

	return BTNodeState::Success;
}
