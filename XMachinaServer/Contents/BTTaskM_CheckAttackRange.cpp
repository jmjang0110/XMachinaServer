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
	if (!mEnemyController->GetTarget()) {
		return BTNodeState::Failure;
	}

	if (mEnemyController->GetMonsterCurrBTType() == FBProtocol::MONSTER_BT_TYPE_ATTACK) {
		return BTNodeState::Success;
	}
	
	// 3. Target Player 가 Cloacking 상태라면 Attack (X)
	SPtr<GameObject> target = mEnemyController->GetTarget();
	if (auto player = std::dynamic_pointer_cast<GamePlayer>(target)) {
		if (player->IsExit() == true) {
			mEnemyController->SetTarget(nullptr);
			return BTNodeState::Failure;
		}

		GameSkill::State IsCloakingOn = player->S_GetSkillState(FBProtocol::PLAYER_SKILL_TYPE_CLOACKING);
		if (IsCloakingOn == GameSkill::State::Active) {
			mEnemyController->SetTarget(nullptr);
			return BTNodeState::Failure;
		}
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
			mStat->StartAttack();

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
