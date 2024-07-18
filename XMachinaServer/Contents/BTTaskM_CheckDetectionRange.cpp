#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	if (!mEnemyController->GetTargetObject()) {
		mEnemyController->SetTargetObject(mTarget);

	}

	///* Ÿ���� �÷��̾ ���� ���¶��.. */
	if (mEnemyController->IsMindControlled() == false) {
		///* Ÿ���� �÷��̾ ���� ���¶��.. */
		bool IsCloakingOn = mEnemyController->GetTargetPlayer()->GetActiveSkill(SkillInfo::Type::Cloaking);
		if (IsCloakingOn == true) {
			mEnemyController->SetTargetPlayer(nullptr);
			return BTNodeState::Failure;
		}
	}

	// ��� ��ã�Ⱑ �������̰ų� ���� ���� ���� ���� ��� ���� ���� ����
	Vec3 TargetPos;
	if (mEnemyController->IsMindControlled() == false) {
		TargetPos = mEnemyController->GetTargetPlayer()->GetPosition();
	}
	else {
		TargetPos = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetPosition();
	}

	if ((GetOwner()->GetTransform()->GetPosition() - TargetPos).Length() < mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Success;
	}
	else {
		mEnemyController->SetTargetObject(nullptr);
	}

	return BTNodeState::Failure;
}



MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}



