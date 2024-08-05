#include "pch.h"
#include "BTTaskM_CheckAttackRange.h"

#include "BTTask.h"
#include "Script_Player.h"
#include "GameMonster.h"
#include "GamePlayer.h"
#include "GameSkill.h"


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
	// 1. Target Object가 없다면 Check attack (X)
	if (!mEnemyController->GetTarget()) {
		return BTNodeState::Failure;
	}

	// 2. Attack 상태라면 CheckAttack Range를 할 필요가 없이 바로 Attack  
	if (mEnemyController->GetState() == EnemyInfo::State::Attack) {
		return BTNodeState::Success;
	}

	
	// 3. Target Player 가 Cloacking 상태라면 Attack (X)
	SPtr<GamePlayer> target = std::dynamic_pointer_cast<GamePlayer>(mEnemyController->GetTarget());
	if (target->IsExit() == true) {
		mEnemyController->SetTarget(nullptr);
		return BTNodeState::Failure;
	}

	GameSkill::State IsCloakingOn = target->GetSNS_SkillState(FBProtocol::PLAYER_SKILL_TYPE_CLOACKING);
	if (IsCloakingOn == GameSkill::State::Active) {
		mEnemyController->SetTarget(nullptr);
		return BTNodeState::Failure;
	}

	
	// 4. Target Player 가 일정 범위에 들어오면 Attack 으로.. 
	constexpr float minDistance    = 1.f;
	Vec3	Pos		               = MonsterBTTask::mTransform->GetPosition();
	auto	targetTransSNS         = target->GetTransform()->GetSnapShot(); /* Snap Shot */
	Vec3	TargetPos              = targetTransSNS.GetPosition();
	float	distance               = (Pos - TargetPos).Length();

	if (distance < mStat->GetStat_AttackRange()) {
		Vec3	ToTargetDir = Vector3::Normalized(TargetPos - Pos);
		float	Angle       = Vector3::Angle(GetOwner()->GetTransform()->GetLook(), ToTargetDir);
		
		// 너무 가까우면 계속 돈다 그래서 적당히 가까우면 Attack State 로 바꾼다.
		if (distance < minDistance || Angle < 80.f) {

			mEnemyController->RemoveAllAnimation();
			MonsterBTTask::mAnimation->GetController()->SetValue("IsAttack", true);
			MonsterBTTask::mAnimation->GetController()->SetValue("Attack", true);

			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK);
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
}

MonsterTask::CheckAttackRange::~CheckAttackRange()
{
	mEnemyController = nullptr;

}
