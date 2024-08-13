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

#include "GameObject.h"
#include "Component.h"
#include "GameMonster.h"
#include "Transform.h"

#include "BTNode.h"
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
#include "Script_Deus_Phase_1.h"
#include "Script_Deus_Phase_2.h"
#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "NPCController.h"

void Script_DefaultEnemyBT::Clone(SPtr<Component> other) 
{
	Script_BehaviorTree::Clone(other);
	SetupTree();
}

bool Script_DefaultEnemyBT::Update()
{
    Script_BehaviorTree::Update();

	if (!mRoot) {
		return true;
	}

	FBProtocol::MONSTER_BT_TYPE PrevType = mRoot->GetEnemyController()->GetMontserPrevBTType();
	FBProtocol::MONSTER_BT_TYPE CurrType = mRoot->GetEnemyController()->GetMonsterCurrBTType();

	if (CurrType == FBProtocol::MONSTER_BT_TYPE_ATTACK)
		return true;

	if (PrevType != CurrType) {

			/* Send Packet */
			switch (CurrType)
			{
			case FBProtocol::MONSTER_BT_TYPE_DEATH:
				LOG_MGR->Cout("MONSTER_BT_TYPE_DEATH\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_ATTACK:
				LOG_MGR->Cout("MONSTER_BT_TYPE_ATTACK\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_GETHIT:
				LOG_MGR->Cout("MONSTER_BT_TYPE_GETHIT\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET:
				LOG_MGR->Cout("MONSTER_BT_TYPE_MOVE_TO_TARGET\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_MOVE_TO_PATH:
				LOG_MGR->Cout("MONSTER_BT_TYPE_MOVE_TO_PATH\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_PATROL:
				LOG_MGR->Cout("MONSTER_BT_TYPE_PATROL\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_IDLE:
				LOG_MGR->Cout("MONSTER_BT_TYPE_IDLE\n");
				break;
			case FBProtocol::MONSTER_BT_TYPE_CHANGE_BT:
				LOG_MGR->Cout("MONSTER_BT_TYPE_CHANGE_BT\n");
				break;
			default:
				LOG_MGR->Cout("MONSTER BT TYPE ..XXXX \n");
				break;
		}

		std::dynamic_pointer_cast<GameMonster>(GetOwner())->Broadcast_SPkt_Mosnter_State(CurrType);
		mRoot->GetEnemyController()->SetBTType(CurrType);
		mRoot->mEnemyController->UpdateMonsterCurrBTType();
	}

	if (mPrevTarget != mRoot->GetEnemyController()->GetTarget()) {
		int monster_id			= GetOwner()->GetID();
		int target_monster_id	= 0;
		int target_player_id	= 0;

		SPtr<GameObject> target = mRoot->GetEnemyController()->GetTarget();
		SPtr<GameObject> invoker = mRoot->GetEnemyController()->GetInvoker();
		if (target) {
			GameObjectInfo::Type objType = target->GetType();
			if (objType == GameObjectInfo::Type::GamePlayer) {
				target_player_id = target->GetID();
			}
			else {
				target_monster_id = target->GetID();
			}
		}
		else if (invoker) {
			target_player_id = invoker->GetID();
		}

		auto pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, target_player_id, target_monster_id);
		GAME_MGR->BroadcastRoom(mRoot->GetEnemyController()->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetID(), pkt);
		mPrevTarget = target;
	}

    return true;
}

BTNode* Script_DefaultEnemyBT::SetupTree()
{
	SPtr<Script_Enemy> enemy = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

	mRoot =
		MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckDeath>(GetOwner(), std::bind(&Script_Enemy::Dead, enemy)),
			MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckAttackRange>(GetOwner()),
				MEMORY->New<MonsterTask::Attack>(GetOwner(), std::bind(&Script_Enemy::Attack, enemy))}),
			MEMORY->New<MonsterTask::GetHit>(GetOwner()),
			MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDetectionRange>(GetOwner()),
				MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
				MEMORY->New<MonsterTask::MoveToTarget>(GetOwner())})})});

	mRoot->SetRoot();
	return mRoot;
}

BTNode* Script_MindControlledEnemyBT::SetupTree()
{
	SPtr<Script_Enemy> enemy = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

	mRoot =
	MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
	MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckMindControlBT>(GetOwner()),
			MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDeath>(GetOwner(), std::bind(&Script_Enemy::Dead, enemy)),
				MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
				MEMORY->New<MonsterTask::CheckAttackRange>(GetOwner()),
					MEMORY->New<MonsterTask::Attack>(GetOwner(), std::bind(&Script_Enemy::Attack, enemy))}),
				MEMORY->New<MonsterTask::GetHit>(GetOwner()),
				MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
				MEMORY->New<MonsterTask::CheckMindDetectionRange>(GetOwner()),
					MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
					MEMORY->New<MonsterTask::MoveToMindControlInvoker>(GetOwner())})})}),
			MEMORY->New<MonsterTask::CheckMindControlBTToEnd>(GetOwner())}),
		MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckDeath>(GetOwner(), std::bind(&Script_Enemy::Dead, enemy)),
			MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckAttackRange>(GetOwner()),
				MEMORY->New<MonsterTask::Attack>(GetOwner(), std::bind(&Script_Enemy::Attack, enemy))}),
			MEMORY->New<MonsterTask::GetHit>(GetOwner()),
			MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDetectionRange>(GetOwner()),
				MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
				MEMORY->New<MonsterTask::MoveToTarget>(GetOwner())})})})});

	mRoot->SetRoot();
	return mRoot;
}

BTNode* Script_DeusPhase1BT::SetupTree()
{
	SPtr<Script_Enemy> enemy = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

	mRoot =
		MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
		MEMORY->New<MonsterTask::CheckDeath>(GetOwner(), std::bind(&Script_Enemy::Dead, enemy)),
			MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckAttackRange>(GetOwner()),
				MEMORY->New<MonsterTask::Attack>(GetOwner(), std::bind(&Script_Enemy::Attack, enemy))}),
			MEMORY->New<MonsterTask::GetHit>(GetOwner()),
			MEMORY->New<BTNode_Sequence>(GetOwner(), std::vector<BTNode*>{
			MEMORY->New<MonsterTask::CheckDetectionRange>(GetOwner()),
				MEMORY->New<BTNode_Selector>(GetOwner(), std::vector<BTNode*>{
				MEMORY->New<MonsterTask::MoveToTarget>(GetOwner())})})});

	mRoot->SetRoot();
	return mRoot;
}
