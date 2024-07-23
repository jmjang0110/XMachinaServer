#include "pch.h"
#include "BTTask.h"
#include "Skill.h"
#include "Script_Player.h"
#include "GameMonster.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Ursacetus.h"
#include "Script_Onyscidus.h"
#include "Script_Arack.h"
#include "Script_Aranobot.h"
#include "Script_Ceratoferox.h"
#include "Script_Gobbler.h"
#include "Script_LightBipedMech.h"
#include "Script_Rapax.h"
#include "Script_Anglerox.h"
#include "Script_MiningMech.h"
/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Attack Range 
/// __________________________________________________________________________
BTNodeState MonsterTask::CheckAttackRange::Evaluate()
{
	//LOG_MGR->Cout("CheckAttackRange\n");
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
		///* 타겟한 플레이어가 은신 상태라면.. */
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
	const auto& o1 = GetOwner();
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(GetOwner()->GetType());


	const auto& motion = GetOwner()->GetAnimation()->GetController()->FindMotionByName(mStat->GetStat_AttackAnimName());
	if (motion) {
		motion->AddEndCallback(std::bind(&CheckAttackRange::AttackEndCallback, this));
	}
	else {
		LOG_MGR->Cout("[ERROR] Couldn't find attack motion : ", GetOwner()->GetName(), " - ",  mStat->GetStat_AttackAnimName(), "\n");
	}
}

MonsterTask::CheckAttackRange::~CheckAttackRange()
{
	mEnemyController = nullptr;

}

void MonsterTask::CheckAttackRange::AttackEndCallback()
{
	GetOwner()->GetAnimation()->GetController()->SetValue("Attack", false);
	mEnemyController->SetState(EnemyInfo::State::Idle);

}
