#pragma once
#include "BTNode.h"
#include "AStarPath.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "GameObject.h"


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

	MonT_CheckAttackRange,
	MonT_CheckDeath,
	MonT_CheckDetectionRange,
	MonT_CheckMindDetectionRange,
	MonT_CheckPatrolRange,

	End,

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
	BTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback = nullptr);
	~BTTask();

};



namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Attack 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Attack : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

	public:
		virtual BTNodeState Evaluate() override;

	public:
		Attack(SPtr_GameObject owner,std::function<void()> callback);
		~Attack();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Get Hit 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class GetHit : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
		float						  mPrevHp{};
		float						  mKnockBack{};

	public:
		virtual BTNodeState Evaluate() override;

		 
	public:
		GetHit(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~GetHit();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º TAsk Patrol 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Patrol : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
	public:
		virtual BTNodeState Evaluate() override;

	public:
		Patrol(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~Patrol();
	};


	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Move To Path
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToPath : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

		float						  mMoveSpeed{};
		float						  mReturnSpeed{};
		std::stack<Vec3>*			  mPath;

	public:
		virtual BTNodeState Evaluate() override;

	public:
		MoveToPath(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~MoveToPath();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Move To Target
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToTarget : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

	public:
		virtual BTNodeState Evaluate() override;


	public:
		MoveToTarget(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~MoveToTarget();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Path Planning A*
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanning_AStar : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;


	private:
		std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

		std::stack<Vec3>*				 mPath;
		Path::Pos						mStart;
		Path::Pos						mDest;

		int								mkWeight = 10;
		int								mkMaxVisited = 2000;

	public:
		virtual BTNodeState Evaluate() override;
		bool PathPlanningAStar(Path::Pos start, Path::Pos dest);
		Path::Pos FindNoneTileFromBfs(const Path::Pos& pos);

	public:
		PathPlanning_AStar(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback = nullptr);

		PathPlanning_AStar(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~PathPlanning_AStar();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Path Planning To Spawn  
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanningToSapwn : public PathPlanning_AStar {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

		Vec3						  mSpawnPos{};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanningToSapwn(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~PathPlanningToSapwn();
	};

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Path Planning To Target 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanningToTarget : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
	public:
		virtual BTNodeState Evaluate() override;

	public:
		PathPlanningToTarget(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~PathPlanningToTarget();
	};


	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Attack Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckAttackRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckAttackRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckAttackRange();
	};


	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Death 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckDeath : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
		float mAccTime{};
		float mRemoveTime{};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckDeath(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckDeath();
	};

	
	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Detection Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckDetectionRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

		SPtr<GameObject>			mTarget;


	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckDetectionRange();
	};
	

	/// +-------------------------------------------------------------------------
	///	> ¢º¢º¢º Task Check Mind Detection Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckMindDetectionRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

	private:
		bool SetTargetNearestEnemy();
	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckMindDetectionRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckMindDetectionRange();
	};

	/// +-------------------------------------------------------------------------
///	> ¢º¢º¢º Task Check Patrol Range
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckPatrolRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckPatrolRange();
	};
}
