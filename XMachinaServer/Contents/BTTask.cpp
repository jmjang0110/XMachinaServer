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
///	> ������ Task Attack 
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
///	> ������ Task Get Hit  
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
///	> ������ Task Move To Path  
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
///	> ������ Task Move To Target  
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
///	> ������ Task Path Planning A*  
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
///	> ������ Task Path Planning To Spawn 
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
///	> ������ Task Planning To Target  
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
///	> ������ Task Patrol 
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
