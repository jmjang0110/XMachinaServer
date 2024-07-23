#pragma once
#include "BTNode.h"
#include "AStarPath.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "GameObject.h"
#include "Animation.h"



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
	SPtr<Script_Enemy> GetStat(GameObjectInfo::Type enemyType);


public:
	BTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback = nullptr);
	~BTTask();

};



namespace MonsterTask {

	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Attack 
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
	///	> ▶▶▶ Task Get Hit 
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
	///	> ▶▶▶ TAsk Patrol 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class Patrol : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

	private:
		std::vector<Vec3>	mWayPoints{};
		int					mCurrWayPointIdx{};
		float				mPatrolSpeed{};

	public:
		virtual BTNodeState Evaluate() override;
		void SetWayPoints(std::vector<Vec3>&& wayPoints);

	public:
		Patrol(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~Patrol();
	};


	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Move To Path
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToPath : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

		float						  mMoveSpeed{};
		float						  mReturnSpeed{};
		std::stack<Vec3>*			  mPath;

		const AnimatorParameter*			  mReturnParam{};

	public:
		virtual BTNodeState Evaluate() override;
		// x절편이 음수인지 양수인지 확인하는 함수 정의
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);
	public:
		MoveToPath(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~MoveToPath();
	};

	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Move To Target
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class MoveToTarget : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

	public:
		virtual BTNodeState Evaluate() override;
		// x절편이 음수인지 양수인지 확인하는 함수 정의
		bool isXInterceptPositive(const Vec3& To, const Vec3& From);


	public:
		MoveToTarget(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~MoveToTarget();
	};

	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Path Planning A*
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanning_AStar : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;


	protected:
		std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

		std::stack<Vec3>*				mPath;
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
	///	> ▶▶▶ Task Path Planning To Spawn  
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
	///	> ▶▶▶ Task Path Planning To Target 
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class PathPlanningToTarget : public PathPlanning_AStar {
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
	///	> ▶▶▶ Task Check Attack Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckAttackRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;
	public:
		virtual BTNodeState Evaluate() override;
		void AttackEndCallback();


	public:
		CheckAttackRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckAttackRange();
	};


	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Check Death 
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
		void ExecuteCallback_CheckDeath();

	public:
		CheckDeath(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckDeath();
	};

	
	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Check Detection Range
	/// __________________________________________________________________________
	/// -------------------------------------------------------------------------+

	class CheckDetectionRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

		SPtr<GamePlayer>			mTargetPlayer;
		SPtr<GameMonster>			mTargetMonster;

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckDetectionRange();
	};
	

	/// +-------------------------------------------------------------------------
	///	> ▶▶▶ Task Check Mind Detection Range
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
///	> ▶▶▶ Task Check Patrol Range
/// __________________________________________________________________________
/// -------------------------------------------------------------------------+

	class CheckPatrolRange : public BTTask {
	private:
		SPtr<Script_EnemyController>  mEnemyController;
		SPtr<Script_Enemy>			  mStat;

		Vec3  mSpawnPos{};
		Vec3  mBaryCenter{};
		float mPatrolRange{};

	public:
		virtual BTNodeState Evaluate() override;

	public:
		CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback = nullptr);
		~CheckPatrolRange();
	};
}
