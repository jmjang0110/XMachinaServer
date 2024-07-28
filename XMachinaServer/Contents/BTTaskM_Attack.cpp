#include "pch.h"
#include "BTTaskM_Attack.h"

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
///	> ������ Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{

	// �� Attack Range BTNode ���� Target üũ�ϹǷ� ���⼭�� �� �ʿ� ���� 

	SPtr<GamePlayer> target = std::dynamic_pointer_cast<GamePlayer>(mEnemyController->GetTarget());
	Vec3 TargetPos          = target->GetPosition(); /* Lock */
	
	// 1. Target Player �� ���� ȸ���Ѵ�.
	GetOwner()->GetTransform()->RotateTargetAxisY(TargetPos, mStat->GetStat_AttackRotationSpeed());
	mStat->UpdatePrevHP();
	ExecuteCallback();

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK);
	return BTNodeState::Success;
}


MonsterTask::Attack::Attack(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_Attack, callback)
{
}


MonsterTask::Attack::~Attack()
{
	mEnemyController = nullptr;

}

