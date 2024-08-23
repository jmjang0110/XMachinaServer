#include "pch.h"
#include "BTTask.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_EnemyController.h"
#include "Script_Enemy.h"

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
#include "Script_Deus_Phase_1.h"
#include "Script_Deus_Phase_2.h"
/* Script */

/// +----------------------------------------------------------------------------------
///		BT TASK 
/// ----------------------------------------------------------------------------------+

BTNodeState BTTask::Evaluate()
{
	return BTNodeState();
}

BTTask::BTTask(SPtr<GameObject> owner, BTTaskType type, std::function<void()> callback)
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

MonsterBTTask::MonsterBTTask(SPtr<GameObject> owner, BTTaskType type, std::function<void()> callback)
	: BTTask(owner, type , callback)
{
	mEnemyController    = mOwner->GetScript<Script_EnemyController>();
	mStat               = mOwner->GetScriptEntity<Script_Enemy>();
	mAnimation          = mOwner->GetAnimation();
	mTransform          = mOwner->GetTransform();

}

MonsterBTTask::~MonsterBTTask()
{
	mEnemyController = nullptr;
	mStat            = nullptr;
	mAnimation       = nullptr;
	mTransform       = nullptr;
}
