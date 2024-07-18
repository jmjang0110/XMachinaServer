#include "pch.h"
#include "BTTask.h"
#include "Skill.h"
#include "Script_Player.h"
/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Attack Range 
/// __________________________________________________________________________
BTNodeState MonsterTask::CheckAttackRange::Evaluate()
{
	LOG_MGR->Cout("CheckAttackRange\n");

	if (!mEnemyController->GetTargetObject()) {
		return BTNodeState::Failure;
	}

	if (mEnemyController->GetState() == EnemyInfo::State::Attack) {
		return BTNodeState::Success;
	}

	///* 타겟한 플레이어가 은신 상태라면.. */
	const auto& playerScript = mEnemyController->GetTargetObject()->GetScript<Script_Player>(ScriptInfo::Type::Stat);
	Skill* cloacking = playerScript->GetSkill(SkillInfo::Type::Cloaking);
	if (cloacking->GetState() == SkillInfo::State::Active) {
		mEnemyController->SetTargetObject(nullptr);
		return BTNodeState::Failure;
	}



	constexpr float minDistance = 1.f;
	const float distance = (GetOwner()->GetTransform()->GetPosition() - mEnemyController->GetTargetObject()->GetTransform()->GetPosition()).Length();
	if (distance < mStat->GetStat_AttackRange()) {

		Vec3		 TargetPos = mEnemyController->GetTargetObject()->GetTransform()->GetPosition();
		Vec3		 MyPos = GetOwner()->GetTransform()->GetPosition();
		const Vec3	 toTargetDir = Vector3::Normalized(TargetPos - MyPos);

		const float	 angle = Vector3::Angle(GetOwner()->GetTransform()->GetLook(), toTargetDir);
		if (minDistance < 1.f || angle < 80.f) {
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
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);
}

MonsterTask::CheckAttackRange::~CheckAttackRange()
{
	mEnemyController = nullptr;

}