#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{
	LOG_MGR->Cout("Attack \n");

	GetOwner()->GetTransform()->RotateTargetAxisY(mEnemyController->GetTargetObject()->GetTransform()->GetPosition(), mStat->GetStat_AttackRotationSpeed());
	mStat->UpdatePrevHP();
	ExecuteCallback();

	return BTNodeState::Success;
}


MonsterTask::Attack::Attack(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Attack, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::Attack::~Attack()
{
	mEnemyController = nullptr;

}

