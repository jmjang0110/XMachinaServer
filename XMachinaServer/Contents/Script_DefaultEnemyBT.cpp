#include "pch.h"
#include "Script_DefaultEnemyBT.h"
#include "Script_Enemy.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

#include "BTNode.h"
#include "BTTask.h"

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

	SPtr<Script_Enemy> enemy = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

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
			SelNode = MEMORY->New<BTNode_Sequence>(GetOwner(), selector_Children);
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
			BTNode* n1 = MEMORY->New<MonsterTask::CheckPatrolRange>(GetOwner()); sequence_children.push_back(n1);
			BTNode* n2 = MEMORY->New<MonsterTask::Patrol>(GetOwner());			 sequence_children.push_back(n2);

			SeqNode = MEMORY->New<BTNode_Sequence>(GetOwner(), sequence_children);
			selector_Children.push_back(SeqNode);
		}
		BTNode* n3 = MEMORY->New<MonsterTask::PathPlanningToSapwn>(GetOwner());	selector_Children.push_back(n3);

		SelNode = MEMORY->New<BTNode_Sequence>(GetOwner(), selector_Children);
		root_selector_Children.push_back(SelNode);
	}

	BTNode* Root = MEMORY->New<BTNode_Selector>(GetOwner(), root_selector_Children);
	return Root;
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

    return false;
}

bool Script_DefaultEnemyBT::Update()
{
    Script_BehaviorTree::Update();

    return false;
}

void Script_DefaultEnemyBT::OnDestroy()
{
    Script_BehaviorTree::OnDestroy();

}
