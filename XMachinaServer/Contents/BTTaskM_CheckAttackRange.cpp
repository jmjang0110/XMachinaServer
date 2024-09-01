#include "pch.h"
#include "BTTaskM_CheckAttackRange.h"
#include "BTTask.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_Player.h"
#include "Script_Skill.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"

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
	
	// 3. Target Player 가 Cloacking or Exit 상태 라면 Attack (X)
	SPtr<GameObject> target        = mEnemyController->GetTarget();
	auto			 player_entity = target->GetScriptEntity<Script_Player>();
	if (player_entity) {

		if (player_entity->GetCurrState() == PlayerState::Exit) {
			mEnemyController->RemoveAllAnimation();
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
			mEnemyController->SetTarget(nullptr);
			return BTNodeState::Failure;
		}

		auto		skill_entity  = player_entity->GetSkillEntity(FBProtocol::PLAYER_SKILL_TYPE_CLOACKING);
		SkillState currSkillState = skill_entity->GetCurrSkillState();
		if (currSkillState == SkillState::Active) {
			mEnemyController->RemoveAllAnimation();
			mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
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
		float	Angle       = Vector3::Angle(mOwner->GetTransform()->GetLook(), ToTargetDir);
		
		// 너무 가까우면 계속 돈다 그래서 적당히 가까우면 Attack State 로 바꾼다.
		if (distance < minDistance || Angle < 10.f) {
			mStat->StartAttack();

			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}

MonsterTask::CheckAttackRange::CheckAttackRange(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckAttackRange, callback)

{
}

MonsterTask::CheckAttackRange::~CheckAttackRange()
{
	mEnemyController = nullptr;

}
