#include "pch.h"
#include "Script_BehaviorTrees.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"

#include "BTTaskM_CheckDeath.h"
#include "BTTaskM_CheckAttackRange.h"
#include "BTTaskM_Attack.h"
#include "BTTaskM_CheckDetectionRange.h"
#include "BTTaskM_CheckPatrolRange.h"
#include "BTTaskM_GetHit.h"
#include "BTTaskM_MoveToPath.h"
#include "BTTaskM_MoveToTarget.h"
#include "BTTaskM_PathPlanningToSpawn.h"
#include "BTTaskM_PathPlanning_AStar.h"
#include "BTTaskM_PathPlanningToTarget.h"
#include "BTTaskM_Patrol.h"
#include "BTTaskM_MoveToMindControlInvoker.h"
#include "BTTaskM_CheckMindControlBT.h"
#include "BTTaskM_CheckMindControlBTToEnd.h"
#include "BTTaskM_CheckMindDetectionRange.h"

#include "FBsPacketFactory.h"
#include "RoomManager.h"
#include "NPCController.h"

#include "GameObject.h"
void Script_DefaultEnemyBT::Update()
{
    Script_BehaviorTree::Update();

	if (!mRoot) {
		return;
	}

	// ¡å 1. Send Monster State Server Packet 
	FBProtocol::MONSTER_BT_TYPE PrevType = mRoot->GetEnemyController()->GetMontserPrevBTType();
	FBProtocol::MONSTER_BT_TYPE CurrType = mRoot->GetEnemyController()->GetMonsterCurrBTType();

	Vec3 pos = mOwner->GetTransform()->GetSnapShot().GetPosition();
	LOG_MGR->Cout(mOwner->GetID(), " : ", pos.x, " ", pos.y, " ", pos.z, "\n");

	if (CurrType == FBProtocol::MONSTER_BT_TYPE_ATTACK)
		return;

	if (PrevType != CurrType) {

		/* Send Packet */
		auto spkt_MonsterState = FBS_FACTORY->SPkt_Monster_State(mOwner->GetID(), CurrType);
		ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), spkt_MonsterState);

		mRoot->GetEnemyController()->SetBTType(CurrType);
		mRoot->mEnemyController->SetMonsterCurrBTType(CurrType);
	}

	// ¡å 2. Send Monster Target Player / Monster(MindControlled) Server Packet 
	if (mRoot->GetEnemyController()->GetPrevTarget() != mRoot->GetEnemyController()->GetTarget()) {
		int monster_id			= mOwner->GetID();
		int target_monster_id	= 0;
		int target_player_id	= 0;

		SPtr<GameObject> target  = mRoot->GetEnemyController()->GetTarget();
		SPtr<GameObject> invoker = mRoot->GetEnemyController()->GetInvoker();

		if (target) {
			if (target->GetTag() == ObjectTag::Player)	
				target_player_id  = target->GetID(); 
			else
				target_monster_id = target->GetID(); 
		}
		else if (invoker)				
			target_player_id  = invoker->GetID(); 

		auto pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, target_player_id, target_monster_id);
		ROOM_MGR->BroadcastRoom(mRoot->GetEnemyController()->GetOwnerRoom()->GetID(), pkt);
		mRoot->GetEnemyController()->SetTarget(target); // Update PrevTarget & Target sync

	}

}

void Script_DefaultEnemyBT::PrintBTtype(FBProtocol::MONSTER_BT_TYPE bttype, std::string substring)
{
	switch (bttype)
	{
	case FBProtocol::MONSTER_BT_TYPE_DEATH:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_DEATH\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_ATTACK:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_ATTACK\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_GETHIT:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_GETHIT\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_MOVE_TO_TARGET\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_MOVE_TO_PATH:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_MOVE_TO_PATH\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_PATROL:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_PATROL\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_IDLE:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_IDLE\n");
		break;
	case FBProtocol::MONSTER_BT_TYPE_CHANGE_BT:
		LOG_MGR->Cout(substring, "MONSTER_BT_TYPE_CHANGE_BT\n");
		break;
	default:
		LOG_MGR->Cout(substring, "MONSTER BT TYPE ..XXXX \n");
		break;
	};
}

SPtr<Component> Script_DefaultEnemyBT::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_DefaultEnemyBT
	auto clonedScript = std::dynamic_pointer_cast<Script_DefaultEnemyBT>(target);
	if (clonedScript)
	{
		// Call the base class Clone method first
		Script_BehaviorTree::Clone(clonedScript);	
		SetupTree();

		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_DefaultEnemyBT" << std::endl;
		return nullptr;
	}
}

void Script_DefaultEnemyBT::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_DefaultEnemyBT>();
	this->Clone(clonedScript);
}

SPtr<Component> Script_MindControlledEnemyBT::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_MindControlledEnemyBT
	auto clonedScript = std::dynamic_pointer_cast<Script_MindControlledEnemyBT>(target);
	if (clonedScript)
	{
		Script_DefaultEnemyBT::Clone(clonedScript);
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_MindControlledEnemyBT" << std::endl;
		return nullptr;
	}
}


void Script_MindControlledEnemyBT::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_MindControlledEnemyBT>();
	this->Clone(clonedScript);
}

SPtr<Component> Script_DeusPhase1BT::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_DeusPhase1BT
	auto clonedScript = std::dynamic_pointer_cast<Script_DeusPhase1BT>(target);
	if (clonedScript)
	{
		Script_DefaultEnemyBT::Clone(clonedScript);
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_DeusPhase1BT" << std::endl;
		return nullptr;
	}
}

void Script_DeusPhase1BT::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_DeusPhase1BT>();
	this->Clone(clonedScript);
}


BTNode* Script_DefaultEnemyBT::SetupTree()
{
	SPtr<Script_Enemy> enemy = mOwner->GetScriptEntity<Script_Enemy>();

	mRoot =
		MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckDeath>(mOwner, std::bind(&Script_Enemy::Dead, enemy)),
			MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckAttackRange>(mOwner),
				MEMORY->New<MonsterTask::Attack>(mOwner, std::bind(&Script_Enemy::Attack, enemy))}),
			MEMORY->New<MonsterTask::GetHit>(mOwner),
			MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDetectionRange>(mOwner),
				MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
				MEMORY->New<MonsterTask::MoveToTarget>(mOwner)})})});

	mRoot->SetRoot();
	return mRoot;
}

BTNode* Script_MindControlledEnemyBT::SetupTree()
{
	SPtr<Script_Enemy> enemy = mOwner->GetScriptEntity<Script_Enemy>();

	mRoot =
	MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
	MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckMindControlBT>(mOwner),
			MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDeath>(mOwner, std::bind(&Script_Enemy::Dead, enemy)),
				MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
				MEMORY->New<MonsterTask::CheckAttackRange>(mOwner),
					MEMORY->New<MonsterTask::Attack>(mOwner, std::bind(&Script_Enemy::Attack, enemy))}),
				MEMORY->New<MonsterTask::GetHit>(mOwner),
				MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
				MEMORY->New<MonsterTask::CheckMindDetectionRange>(mOwner),
					MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
					MEMORY->New<MonsterTask::MoveToMindControlInvoker>(mOwner)})})}),
			MEMORY->New<MonsterTask::CheckMindControlBTToEnd>(mOwner)}),
		MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckDeath>(mOwner, std::bind(&Script_Enemy::Dead, enemy)),
			MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckAttackRange>(mOwner),
				MEMORY->New<MonsterTask::Attack>(mOwner, std::bind(&Script_Enemy::Attack, enemy))}),
			MEMORY->New<MonsterTask::GetHit>(mOwner),
			MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDetectionRange>(mOwner),
				MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
				MEMORY->New<MonsterTask::MoveToTarget>(mOwner)})})})});

	mRoot->SetRoot();
	return mRoot;
}

BTNode* Script_DeusPhase1BT::SetupTree()
{
	SPtr<Script_Enemy> enemy = mOwner->GetScriptEntity<Script_Enemy>();

	mRoot =
		MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckDeath>(mOwner, std::bind(&Script_Enemy::Dead, enemy)),
			MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckAttackRange>(mOwner),
				MEMORY->New<MonsterTask::Attack>(mOwner, std::bind(&Script_Enemy::Attack, enemy))}),
			MEMORY->New<MonsterTask::GetHit>(mOwner),
			MEMORY->New<BTNode_Sequence>(mOwner, std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDetectionRange>(mOwner),
				MEMORY->New<BTNode_Selector>(mOwner, std::vector<BTNode*>{
				MEMORY->New<MonsterTask::MoveToTarget>(mOwner)})})});

	mRoot->SetRoot();
	return mRoot;
}
