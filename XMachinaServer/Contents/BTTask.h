#pragma once
#include "BTNode.h"

enum class BTTaskType : UINT16 {
	/* Monster Task */
	MonT_Attack,
	MonT_GetHit,
	MonT_Patrol,
	MonT_MoveToPath,
	MonT_MoveToTarget,
	MonT_PathPlanningASatr,
	MonT_PathPlanningToSpawn,
	MonT_PathPlanningToTarget,

};

class BTTask : public BTNode_Action
{
private:

public:
	virtual BTNodeState Evaluate() override;

public:
	BTTask();
	~BTTask();

};

namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ������ Task Attack 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Attack : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		Attack();
		~Attack();
	};

	/// +-------------------------------------------------------------------------
	///	> ������ Task Get Hit 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class GetHit : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		GetHit();
		~GetHit();
	};

	/// +-------------------------------------------------------------------------
	///	> ������ TAsk Patrol 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Patrol : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		Patrol();
		~Patrol();
	};


	/// +-------------------------------------------------------------------------
	///	> ������ Task Move To Path
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToPath : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		MoveToPath();
		~MoveToPath();
	};

	/// +-------------------------------------------------------------------------
	///	> ������ Task Move To Target
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToTarget : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		MoveToTarget();
		~MoveToTarget();
	};

	/// +-------------------------------------------------------------------------
	///	> ������ Task Path Planning A*
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanning_AStar : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanning_AStar();
		~PathPlanning_AStar();
	};

	/// +-------------------------------------------------------------------------
	///	> ������ Task Path Planning To Spawn  
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanningToSapwn : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanningToSapwn();
		~PathPlanningToSapwn();
	};

	/// +-------------------------------------------------------------------------
	///	> ������ Task Path Planning To Target 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanningToTarget : public BTTask {
	private:

	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanningToTarget();
		~PathPlanningToTarget();
	};


}
