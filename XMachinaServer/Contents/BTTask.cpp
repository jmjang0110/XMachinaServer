#include "pch.h"
#include "BTTask.h"

BTTask::BTTask()
{
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
{
}

MonsterTask::Patrol::~Patrol()
{
}
