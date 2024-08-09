#include "pch.h"
#include "BTTask.h"

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

/* Script */

/// +----------------------------------------------------------------------------------
///		BT TASK 
/// ----------------------------------------------------------------------------------+

BTNodeState BTTask::Evaluate()
{
	return BTNodeState();
}

SPtr<Script_Enemy> BTTask::GetStat(GameObjectInfo::Type enemyType)
{
	SPtr<Script_Enemy> stat;
	if (enemyType == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		stat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Onyscidus)
		stat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Ursacetus)
		stat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Anglerox)
		stat = GetOwner()->GetScript<Script_Anglerox>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Arack)
		stat = GetOwner()->GetScript<Script_Arack>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Aranobot)
		stat = GetOwner()->GetScript<Script_Aranobot>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Ceratoferox)
		stat = GetOwner()->GetScript<Script_Ceratoferox>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Gobbler)
		stat = GetOwner()->GetScript<Script_Gobbler>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_LightBipedMech)
		stat = GetOwner()->GetScript<Script_LightBipedMech>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_MiningMech)
		stat = GetOwner()->GetScript<Script_MiningMech>(ScriptInfo::Type::Stat);

	else if (enemyType == GameObjectInfo::Type::Monster_Rapax)
		stat = GetOwner()->GetScript<Script_Rapax>(ScriptInfo::Type::Stat);

	return stat;
}

BTTask::BTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback)
	: BTNode_Action(owner, callback)
{
	mType = type;
	
}

BTTask::~BTTask()
{
}

/// +----------------------------------------------------------------------------------
///		BT TASK 
/// ----------------------------------------------------------------------------------+

MonsterBTTask::MonsterBTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback)
	: BTTask(owner, type , callback)
{
	mEnemyController	= GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat				= GetStat(owner->GetType());
	mAnimation          = GetOwner()->GetAnimation();
	mTransform          = GetOwner()->GetTransform();

}

MonsterBTTask::~MonsterBTTask()
{
	mEnemyController = nullptr;
	mStat            = nullptr;
	mAnimation       = nullptr;
	mTransform       = nullptr;
}
