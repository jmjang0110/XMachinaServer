#include "pch.h"
#include "Script_DefaultEnemyBT.h"
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

#include "GameObject.h"
#include "Component.h"
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
#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "NPCController.h"


BTNode* Script_DefaultEnemyBT::SetupTree()
{

#pragma region WayPoint
	// 오브젝트 스폰 위치 중심 (5, 5) 만큼 정찰 위치 설정
	std::vector<Vec3> wayPoints(3);
	const float waySize = 5.f;
	wayPoints[0] = GetOwner()->GetTransform()->GetLocalPosition();
	wayPoints[1] = GetOwner()->GetTransform()->GetLocalPosition() + Vec3(waySize, 0.f, 0.f);
	wayPoints[2] = GetOwner()->GetTransform()->GetLocalPosition() + Vec3(waySize, 0.f, waySize);

	// 무게 중심 위치 계산
	const Vec3 baryCenter = std::accumulate(wayPoints.begin(), wayPoints.end(), Vec3{ 0.f, 0.f, 0.f }) / 3.f;

	// 무게 중심으로부터 가장 먼 길이 계산
	float maxDis{};
	for (const auto& wayPoint : wayPoints)
		maxDis = std::max(maxDis, Vec3::Distance(baryCenter, wayPoint));
#pragma endregion
	SPtr<Script_Enemy> enemy = GetScriptEnemy(GetOwner()->GetType());

#pragma region BehaviorTree
	

	/// +---------------------------------------------------------------------------------------------------------------
	///	Ready To Create Behavior Tree 
	/// ---------------------------------------------------------------------------------------------------------------+
	std::vector<BTNode*>	 Selector_Root;
	BTNode*					 CheckDeath				= MEMORY->New<MonsterTask::CheckDeath>(GetOwner(), std::bind(&Script_Enemy::Dead, enemy));
	
	std::vector<BTNode*>	 Sequence_1;
	BTNode*					 CheckAttackRange		= MEMORY->New<MonsterTask::CheckAttackRange>(GetOwner());
	BTNode*					 Attack					= MEMORY->New<MonsterTask::Attack>(GetOwner(), std::bind(&Script_Enemy::Attack, enemy));


	BTNode*					 GetHit					= MEMORY->New<MonsterTask::GetHit>(GetOwner());

	std::vector<BTNode*>     Sequence_2;
	BTNode*					 CheckDetectionRange	= MEMORY->New<MonsterTask::CheckDetectionRange>(GetOwner());

	std::vector<BTNode*>	 Selector_2_1;
	BTNode*					 MoveToTarget			= MEMORY->New<MonsterTask::MoveToTarget>(GetOwner());
	BTNode*					 PathPlanningToTarget	= MEMORY->New<MonsterTask::PathPlanningToTarget>(GetOwner());


	/// +---------------------------------------------------------------------------------------------------------------
	///	Create Behavior Tree 
	/// ---------------------------------------------------------------------------------------------------------------+
	// -- Root -- 
	Selector_Root.push_back(CheckDeath);  // [0] Check Death
	{
		// [1] -- Sequence 1 --
		Sequence_1.push_back(CheckAttackRange);	// [1-1] Check Attack Range 
		Sequence_1.push_back(Attack);			// [1-2] Attack
		BTNode* Sequence_1_Node = MEMORY->New<BTNode_Sequence>(GetOwner(), Sequence_1);
		Selector_Root.push_back(Sequence_1_Node);
	}
	// [2] -- Get Hit -- 
	Selector_Root.push_back(GetHit);
	{	
		// [3] -- Sequence 2 -- 
		Sequence_2.push_back(CheckDetectionRange); // [3-0] Check Detection Range 
		{
			// -- Selector 2_1 --
			Selector_2_1.push_back(MoveToTarget);		  // [3-1] Move To Target 
			Selector_2_1.push_back(PathPlanningToTarget); // [3-2] Path Planning To Target 
			BTNode* Selector_2_1_Node = MEMORY->New<BTNode_Selector>(GetOwner(), Selector_2_1);
			Sequence_2.push_back(Selector_2_1_Node);
		}
		BTNode* Sequence_2_Node = MEMORY->New<BTNode_Sequence>(GetOwner(), Sequence_2);
		Selector_Root.push_back(Sequence_2_Node);
	}
		
	mRoot = MEMORY->New<BTNode_Selector>(GetOwner(), Selector_Root);
	mRoot->SetRoot();
	return mRoot;

	///* CheckAttackRange */
//BTNode* node3 = MEMORY->New<MonsterTask::MoveToPath>(GetOwner()); root_selector_Children.push_back(node3);
//{
//	BTNode* SelNode{};

//	std::vector<BTNode*> selector_Children;
//	{
//		/* Sequence - CheckAttackRange, Attack */
//		BTNode* SeqNode{};
//		
//		std::vector<BTNode*> sequence_children;
//		BTNode* n1 = MEMORY->New<MonsterTask::CheckPatrolRange>(GetOwner(), baryCenter, maxDis); sequence_children.push_back(n1);
//		BTNode* n2 = MEMORY->New<MonsterTask::Patrol>(GetOwner());			 sequence_children.push_back(n2);

//		SeqNode = MEMORY->New<BTNode_Sequence>(GetOwner(), sequence_children);
//		selector_Children.push_back(SeqNode);
//	}
//	BTNode* n3 = MEMORY->New<MonsterTask::PathPlanningToSapwn>(GetOwner());	selector_Children.push_back(n3);

//	SelNode = MEMORY->New<BTNode_Selector>(GetOwner(), selector_Children);
//	root_selector_Children.push_back(SelNode);
//}

}

SPtr<Script_Enemy> Script_DefaultEnemyBT::GetScriptEnemy(GameObjectInfo::Type objtype)
{
	SPtr<Script_Enemy> enemy = {};

	switch (objtype)
	{

	case GameObjectInfo::Type::Monster_Ursacetus: {
		SPtr<Script_Ursacetus> script = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Onyscidus: {
		SPtr<Script_Onyscidus> script = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_AdvancedCombat_5: {
		SPtr<Script_AdvancedCombatDroid_5> script = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Anglerox: {
		SPtr<Script_Anglerox> script = GetOwner()->GetScript<Script_Anglerox>(ScriptInfo::Type::Anglerox);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Arack: {
		SPtr<Script_Arack> script = GetOwner()->GetScript<Script_Arack>(ScriptInfo::Type::Arack);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Ceratoferox: {
		SPtr<Script_Ceratoferox> script = GetOwner()->GetScript<Script_Ceratoferox>(ScriptInfo::Type::Ceratoferox);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Gobbler: {
		SPtr<Script_Gobbler> script = GetOwner()->GetScript<Script_Gobbler>(ScriptInfo::Type::Gobbler);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_LightBipedMech: {
		SPtr<Script_LightBipedMech> script = GetOwner()->GetScript<Script_LightBipedMech>(ScriptInfo::Type::LightBipedMech);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_MiningMech: {
		SPtr<Script_MiningMech> script = GetOwner()->GetScript<Script_MiningMech>(ScriptInfo::Type::MiningMech);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Rapax: {
		SPtr<Script_Rapax> script = GetOwner()->GetScript<Script_Rapax>(ScriptInfo::Type::Rapax);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	case GameObjectInfo::Type::Monster_Aranobot: {
		SPtr<Script_Aranobot> script = GetOwner()->GetScript<Script_Aranobot>(ScriptInfo::Type::Aranabot);
		enemy = std::static_pointer_cast<Script_Enemy>(script);
	}
		break;
	default:
		break;
	}

	return enemy;
}



Script_DefaultEnemyBT::Script_DefaultEnemyBT()
    :   Script_BehaviorTree()
{

}

Script_DefaultEnemyBT::Script_DefaultEnemyBT(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script_BehaviorTree(owner, type)
{

}

Script_DefaultEnemyBT::~Script_DefaultEnemyBT()
{
	MEMORY->Delete(mRoot);
}



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
		default:
			LOG_MGR->Cout("MONSTER BT TYPE ..XXXX \n");
			break;
		}

		std::dynamic_pointer_cast<GameMonster>(GetOwner())->Broadcast_SPkt_Mosnter_State(CurrType);
		mRoot->GetEnemyController()->SetBTType(CurrType);

		bool IsMindControlled = mRoot->GetEnemyController()->IsMindControlled();

		// [BSH] : 타겟이 어떤 몬스터의 것인지에 대한 몬스터 아이디도 보내야함.
		int monster_id		  = GetOwner()->GetID();
		// int monster_id     = -1;

		int target_monster_id = 0;
		int target_player_id  = 0;
		SPtr<GameObject> target = mRoot->GetEnemyController()->GetTarget();
		if (target) {
			GameObjectInfo::Type objType = target->GetType();
			if (objType == GameObjectInfo::Type::GamePlayer) {
				target_player_id = target->GetID();
			}
			else {
				target_monster_id = target->GetID();
			}
		}

		auto pkt = FBS_FACTORY->SPkt_Monster_Target(monster_id, target_player_id, target_monster_id);
		GAME_MGR->BroadcastRoom(mRoot->GetEnemyController()->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetID(), pkt);
	}

	mRoot->GetEnemyController()->UpdateMonsterCurrBTType();

    return true;
}

