#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	bool IsMindControlled = mEnemyController->IsMindControlled();
	if (IsMindControlled == false) {
		if (!mEnemyController->GetTargetPlayer())
			mEnemyController->SetTargetPlayer(mTargetPlayer);
	}
	else {
		if (!mEnemyController->GetTargetMonster())
			mEnemyController->SetTargetMonster(mTargetMonster);
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
	if (IsMindControlled == false) {
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
		if (IsMindControlled == false)
			mEnemyController->SetTargetPlayer(nullptr);
		else
			mEnemyController->SetTargetMonster(nullptr);
	}

	return BTNodeState::Failure;
}



MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);
}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}



