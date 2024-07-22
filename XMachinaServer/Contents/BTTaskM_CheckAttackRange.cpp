#include "pch.h"
#include "BTTask.h"
#include "Skill.h"
#include "Script_Player.h"
#include "GameMonster.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Check Attack Range 
/// __________________________________________________________________________
BTNodeState MonsterTask::CheckAttackRange::Evaluate()
{
	LOG_MGR->Cout("CheckAttackRange\n");
	bool IsMindControlled = mEnemyController->IsMindControlled();
	if (IsMindControlled == false) {
		if (!mEnemyController->GetTargetPlayer())
			return BTNodeState::Failure;
	}
	else {
		if (!mEnemyController->GetTargetMonster())
			return BTNodeState::Failure;
	}


	if (mEnemyController->GetState() == EnemyInfo::State::Attack) {
		return BTNodeState::Success;
	}

	
	if(IsMindControlled == false) {
		///* Ÿ���� �÷��̾ ���� ���¶��.. */
		bool IsCloakingOn = mEnemyController->GetTargetPlayer()->GetActiveSkill(SkillInfo::Type::Cloaking);
		if (IsCloakingOn == true) {
			mEnemyController->SetTargetPlayer(nullptr);
			return BTNodeState::Failure;
		}
	}



	constexpr float minDistance = 1.f;
	Vec3 Monster_Pos = mEnemyController->GetOwnerMonster()->GetTransform()->GetPosition();
	Vec3  TargetPos{};

	if (IsMindControlled == false)
		TargetPos = mEnemyController->GetTargetPlayer()->GetPosition();
	else
		TargetPos = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetPosition();

	float distance = (Monster_Pos - TargetPos).Length();
	if (distance < mStat->GetStat_AttackRange()) {
		Vec3	ToTargetDir = Vector3::Normalized(TargetPos - Monster_Pos);
		float	Angle       = Vector3::Angle(mEnemyController->GetOwnerMonster()->GetTransform()->GetLook(), ToTargetDir);
		if (minDistance < 1.f || Angle < 80.f) {
			mEnemyController->SetState(EnemyInfo::State::Attack);
			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckAttackRange::CheckAttackRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckAttackRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);
}

MonsterTask::CheckAttackRange::~CheckAttackRange()
{
	mEnemyController = nullptr;

}