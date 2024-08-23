#pragma once
#include "BTNode.h"
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
	MonT_CheckMindControlBT,

	End,

};

class GameObject;
class Transform;
class Animation;
class Script_EnemyController;
class Script_Enemy;
class BTTask : public BTNode_Action
{
private:
	BTTaskType mType;

public:
	virtual BTNodeState Evaluate() override;

public:
	BTTaskType GetType() { return mType; }


public:
	BTTask(SPtr<GameObject> owner, BTTaskType type, std::function<void()> callback = nullptr);
	~BTTask();

};

class MonsterBTTask : public BTTask
{
protected:
	SPtr<Script_EnemyController>  mEnemyController;
	SPtr<Script_Enemy>			  mStat;
	SPtr<Animation>				  mAnimation;
	SPtr<Transform>				  mTransform;
public:
	MonsterBTTask(SPtr<GameObject> owner, BTTaskType type, std::function<void()> callback = nullptr);
	~MonsterBTTask();
};

