#include "pch.h"
#include "Script_DefaultEnemyBT.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"


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
	std::vector<BTNode*> root_selector_Children;
	
	/* Check Death */
	BTNode* node1 = MEMORY->New<MonsterTask::CheckDeath>(GetOwner(), std::bind(&Script_Enemy::Dead, enemy));	
	root_selector_Children.push_back(node1);
	{
		/* Sequence - CheckAttackRange, Attack */
		BTNode* SeqNode{};
		
		std::vector<BTNode*> sequence_children;
		BTNode* n1 = MEMORY->New<MonsterTask::CheckAttackRange>(GetOwner());								sequence_children.push_back(n1);
		BTNode* n2 = MEMORY->New<MonsterTask::Attack>(GetOwner(), std::bind(&Script_Enemy::Attack, enemy));	sequence_children.push_back(n2);
		
		SeqNode = MEMORY->New<BTNode_Sequence>(GetOwner(), sequence_children);
		root_selector_Children.push_back(SeqNode);
	}

	/* Get Hit */
	BTNode* node2 = MEMORY->New<MonsterTask::GetHit>(GetOwner());
	root_selector_Children.push_back(node2);


	{
		/* Sequence - CheckDetectionRange, */
		BTNode* SeqNode{};

		std::vector<BTNode*> sequence_children;
		BTNode* n1 = MEMORY->New<MonsterTask::CheckDetectionRange>(GetOwner());	sequence_children.push_back(n1);
		{
			/* Selector - TaskMoveToTArget, TaskPathPlanningToTarget */
			BTNode* SelNode{};

			std::vector<BTNode*> selector_Children;
			BTNode* n1 = MEMORY->New<MonsterTask::MoveToTarget>(GetOwner());		 selector_Children.push_back(n1);
			BTNode* n2 = MEMORY->New<MonsterTask::PathPlanningToTarget>(GetOwner()); selector_Children.push_back(n2);
			SelNode = MEMORY->New<BTNode_Selector>(GetOwner(), selector_Children);
			sequence_children.push_back(SelNode);
		}

		SeqNode = MEMORY->New<BTNode_Sequence>(GetOwner(), sequence_children);
		root_selector_Children.push_back(SeqNode);
	}

	/* CheckAttackRange */
	BTNode* node3 = MEMORY->New<MonsterTask::MoveToPath>(GetOwner()); root_selector_Children.push_back(node3);
	{
		BTNode* SelNode{};

		std::vector<BTNode*> selector_Children;
		{
			/* Sequence - CheckAttackRange, Attack */
			BTNode* SeqNode{};
			
			std::vector<BTNode*> sequence_children;
			BTNode* n1 = MEMORY->New<MonsterTask::CheckPatrolRange>(GetOwner(), baryCenter, maxDis); sequence_children.push_back(n1);
			BTNode* n2 = MEMORY->New<MonsterTask::Patrol>(GetOwner());			 sequence_children.push_back(n2);

			SeqNode = MEMORY->New<BTNode_Sequence>(GetOwner(), sequence_children);
			selector_Children.push_back(SeqNode);
		}
		BTNode* n3 = MEMORY->New<MonsterTask::PathPlanningToSapwn>(GetOwner());	selector_Children.push_back(n3);

		SelNode = MEMORY->New<BTNode_Selector>(GetOwner(), selector_Children);
		root_selector_Children.push_back(SelNode);
	}

	mRoot = MEMORY->New<BTNode_Selector>(GetOwner(), root_selector_Children);
	mRoot->SetRoot();
	return mRoot;
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
}

void Script_DefaultEnemyBT::OnEnable()
{
    Script_BehaviorTree::OnEnable();

}

void Script_DefaultEnemyBT::OnDisable()
{
    Script_BehaviorTree::OnDisable();

}

void Script_DefaultEnemyBT::Clone(SPtr<Component> other) 
{
	Script_BehaviorTree::Clone(other);

	SetupTree();

}

void Script_DefaultEnemyBT::Activate()
{
    Script_BehaviorTree::Activate();

}

void Script_DefaultEnemyBT::DeActivate()
{
    Script_BehaviorTree::Activate();

}

bool Script_DefaultEnemyBT::WakeUp()
{
    Script_BehaviorTree::WakeUp();

    return false;
}

bool Script_DefaultEnemyBT::Start()
{
    Script_BehaviorTree::Start();

    return true;
}

bool Script_DefaultEnemyBT::Update()
{
    Script_BehaviorTree::Update();

	if (mRoot) {
		mRoot->Evaluate();

		FBProtocol::MONSTER_BT_TYPE PrevType = mRoot->GetEnemyController()->GetMontserPrevBTType();
		FBProtocol::MONSTER_BT_TYPE CurrType = mRoot->GetEnemyController()->GetMonsterCurrBTType();

		if (PrevType != CurrType) {
			/* Send Packet */
			std::dynamic_pointer_cast<GameMonster>(GetOwner())->Broadcast_SPkt_Mosnter_State(CurrType);
		}

		mRoot->GetEnemyController()->UpdateMonsterCurrBTType();
	}

    return true;
}

void Script_DefaultEnemyBT::OnDestroy()
{
    Script_BehaviorTree::OnDestroy();

}
