#include "pch.h"
#include "BTTask.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Move To Path  
/// __________________________________________________________________________

MonsterTask::MoveToPath::MoveToPath(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_MoveToPath, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

	mMoveSpeed = mStat->GetStat_MoveSpeed();
	mReturnSpeed = 1.7f * mMoveSpeed;
	mPath = mEnemyController->GetPaths();

}

MonsterTask::MoveToPath::~MoveToPath()
{
	mEnemyController = nullptr;

}

BTNodeState MonsterTask::MoveToPath::Evaluate()
{
	if (mPath->empty())
		return BTNodeState::Failure;


	// ���� ��α����� ����
	Vec3 nextPos = (mPath->top() - GetOwner()->GetTransform()->GetPosition()).xz();

	// ���� ���� ���¶�� ���ǵ带 �ø���.
	float speed{};
	//if (mReturnParam->val.b) {
	//	speed = mReturnSpeed;
	//	//mWalkMotion->SetSpeed(1.7f);
	//}
	//else {
	//	speed = mMoveSpeed;
	//	//mWalkMotion->SetSpeed(1.f);
	//}

	// ���� ��θ� ���� �̵� �� ȸ��
	GetOwner()->GetTransform()->RotateTargetAxisY(mPath->top(), mStat->GetStat_RotationSpeed());
	GetOwner()->GetTransform()->Translate(XMVector3Normalize(nextPos), speed * DELTA_TIME);

	// ���� ��ο� ���� �� �ش� ��� ����
	const float kMinDistance = 0.1f;
	if (nextPos.Length() < kMinDistance)
		mPath->pop();

	return BTNodeState::Success;
}
