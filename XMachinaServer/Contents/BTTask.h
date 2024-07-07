#pragma once
#include "BTNode.h"
#include "AStarPath.h"

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
	BTTaskType mType;

public:
	virtual BTNodeState Evaluate() override;

public:
	BTTaskType GetType() { return mType; }

public:
	BTTask();
	BTTask(BTTaskType type);
	~BTTask();

};

namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Attack 
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
	///	> ¢º¢º¢º Task Get Hit 
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
	///	> ¢º¢º¢º TAsk Patrol 
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
	///	> ¢º¢º¢º Task Move To Path
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
	///	> ¢º¢º¢º Task Move To Target
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
	///	> ¢º¢º¢º Task Path Planning A*
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanning_AStar : public BTTask {
	private:
		AStarPath mAStarPath;

	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanning_AStar();
		~PathPlanning_AStar();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Path Planning To Spawn  
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
	///	> ¢º¢º¢º Task Path Planning To Target 
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
