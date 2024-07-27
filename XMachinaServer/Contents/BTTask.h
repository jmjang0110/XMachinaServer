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
	MonT_MoveToMindControlInvoker,
	
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

class MonsterBTTask : public BTTask
{
protected:
	SPtr<Script_EnemyController>  mEnemyController;
	SPtr<Script_Enemy>			  mStat;

public:
	MonsterBTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback = nullptr);
	~MonsterBTTask();
};

