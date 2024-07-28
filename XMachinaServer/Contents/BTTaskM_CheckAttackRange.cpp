#include "pch.h"
#include "BTTaskM_CheckAttackRange.h"

#include "BTTask.h"
#include "Skill.h"
#include "Script_Player.h"
#include "GameMonster.h"
#include "GamePlayer.h"


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
///	> ������ Task Check Attack Range 
/// __________________________________________________________________________
BTNodeState MonsterTask::CheckAttackRange::Evaluate()
{
	// 1. Target Object�� ���ٸ� Check attack (X)
	if (!mEnemyController->GetTarget()) {
		return BTNodeState::Failure;
	}

	// 2. Attack ���¶�� CheckAttack Range�� �� �ʿ䰡 ���� �ٷ� Attack  
	if (mEnemyController->GetState() == EnemyInfo::State::Attack) {
		return BTNodeState::Success;
	}

	
	// 3. Target Player �� Cloacking ���¶�� Attack (X)
	SPtr<GamePlayer> target = std::dynamic_pointer_cast<GamePlayer>(mEnemyController->GetTarget());
	bool IsCloakingOn = target->GetActiveSkill(SkillInfo::Type::Cloaking);
	if (IsCloakingOn == true) {
		mEnemyController->SetTarget(nullptr);
		return BTNodeState::Failure;
	}

	
	// 4. Target Player �� ���� ������ ������ Attack ����.. 
	constexpr float minDistance = 1.f;
	Vec3	Pos		  = GetOwner()->GetTransform()->GetPosition();
	Vec3	TargetPos = target->GetPosition();
	float	distance  = (Pos - TargetPos).Length();

	if (distance < mStat->GetStat_AttackRange()) {
		Vec3	ToTargetDir = Vector3::Normalized(TargetPos - Pos);
		float	Angle       = Vector3::Angle(GetOwner()->GetTransform()->GetLook(), ToTargetDir);
		
		// �ʹ� ������ ��� ���� �׷��� ������ ������ Attack State �� �ٲ۴�.
		if (distance < minDistance || Angle < 80.f) {
			mEnemyController->RemoveAllAnimation();
			GetOwner()->GetAnimation()->GetController()->SetValue("Attack", true);
			mEnemyController->SetState(EnemyInfo::State::Attack);

			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckAttackRange::CheckAttackRange(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckAttackRange, callback)

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
