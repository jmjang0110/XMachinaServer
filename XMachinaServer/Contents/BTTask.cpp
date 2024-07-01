#include "pch.h"
#include "BTTask.h"

BTTask::BTTask()
{
}

BTTask::BTTask(BTTaskType type)
	: BTNode_Action()
{
	mType = type;

}

BTTask::~BTTask()
{
}

BTNodeState BTTask::Evaluate()
{
	return BTNodeState();
}


/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Attack 
/// __________________________________________________________________________

BTNodeState MonsterTask::Attack::Evaluate()
{
	return BTNodeState();
}

MonsterTask::Attack::Attack()
	: BTTask(BTTaskType::MonT_Attack)
{

}

MonsterTask::Attack::~Attack()
{
}



/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Get Hit  
/// __________________________________________________________________________

BTNodeState MonsterTask::GetHit::Evaluate()
{
	return BTNodeState();
}

MonsterTask::GetHit::GetHit()
	: BTTask(BTTaskType::MonT_GetHit)
{
}

MonsterTask::GetHit::~GetHit()
{
}


/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Move To Path  
/// __________________________________________________________________________

BTNodeState MonsterTask::MoveToPath::Evaluate()
{
	return BTNodeState();
}

MonsterTask::MoveToPath::MoveToPath()
	: BTTask(BTTaskType::MonT_MoveToPath)
{
}

MonsterTask::MoveToPath::~MoveToPath()
{
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Move To Target  
/// __________________________________________________________________________

BTNodeState MonsterTask::MoveToTarget::Evaluate()
{
	return BTNodeState();
}

MonsterTask::MoveToTarget::MoveToTarget()
	: BTTask(BTTaskType::MonT_MoveToTarget)
{
}

MonsterTask::MoveToTarget::~MoveToTarget()
{
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Path Planning A*  
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanning_AStar::Evaluate()
{
	return BTNodeState();
}

MonsterTask::PathPlanning_AStar::PathPlanning_AStar()
	: BTTask(BTTaskType::MonT_PathPlanningASatr)
{
}

MonsterTask::PathPlanning_AStar::~PathPlanning_AStar()
{
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Path Planning To Spawn 
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToSapwn::Evaluate()
{
	return BTNodeState();
}

MonsterTask::PathPlanningToSapwn::PathPlanningToSapwn()
	: BTTask(BTTaskType::MonT_PathPlanningToSpawn)
{
}

MonsterTask::PathPlanningToSapwn::~PathPlanningToSapwn()
{
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Planning To Target  
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToTarget::Evaluate()
{
	return BTNodeState();
}

MonsterTask::PathPlanningToTarget::PathPlanningToTarget()
	: BTTask(BTTaskType::MonT_PathPlanningToTarget)
{
}

MonsterTask::PathPlanningToTarget::~PathPlanningToTarget()
{
}

/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Patrol 
/// __________________________________________________________________________

BTNodeState MonsterTask::Patrol::Evaluate()
{
	return BTNodeState();
}

MonsterTask::Patrol::Patrol()
	: BTTask(BTTaskType::MonT_Patrol)
{
}

MonsterTask::Patrol::~Patrol()
{
}
