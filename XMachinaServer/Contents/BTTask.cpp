#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"



/* Script */



BTTask::BTTask(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback)
	: BTNode_Action(owner, callback)
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
///	> ▶▶▶ Task Attack 
/// __________________________________________________________________________

MonsterTask::Attack::Attack(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Attack, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::Attack::~Attack()
{
	mEnemyController = nullptr;

}

BTNodeState MonsterTask::Attack::Evaluate()
{
	LOG_MGR->Cout("Attack \n");

	SPtr<Transform> trans = GetOwner()->GetTransform();


	Vec3  TargetPos = mEnemyController->GetTargetObject()->GetTransform()->GetPosition();
	float AttackRotSpeed = mStat->GetStat_AttackRotationSpeed();
	trans->RotateTargetAxisY(TargetPos, AttackRotSpeed);

	BTNode_Action::ExecuteCallback();

	return BTNodeState::Success;
}





/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Get Hit  
/// __________________________________________________________________________

MonsterTask::GetHit::GetHit(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_GetHit, callback)
{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);
	mPrevHp = mStat->GetCrntHp();
	mKnockBack = 0.05f;
}

MonsterTask::GetHit::~GetHit()
{
	mEnemyController = nullptr;

}


BTNodeState MonsterTask::GetHit::Evaluate()
{
	LOG_MGR->Cout("GetHit \n");

	if (!mEnemyController->GetTargetObject()) {
		return BTNodeState::Failure;
	}

	const float crntHp = mStat->GetCrntHp();
	if (!mStat->UpdatePrevHP()) {
		mEnemyController->SetState(EnemyInfo::State::GetHit);

		Vec3 TargetLook = mEnemyController->GetTargetObject()->GetTransform()->GetLook();
		GetOwner()->GetTransform()->Translate(TargetLook, mKnockBack);
	}

	return BTNodeState::Success;
}



/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Move To Path  
/// __________________________________________________________________________

MonsterTask::MoveToPath::MoveToPath(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_MoveToPath, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat            = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

	mMoveSpeed       = mStat->GetStat_MoveSpeed();
	mReturnSpeed     = 1.7f * mMoveSpeed;
	mPath            = mEnemyController->GetPaths();

}

MonsterTask::MoveToPath::~MoveToPath()
{
	mEnemyController = nullptr;

}

BTNodeState MonsterTask::MoveToPath::Evaluate()
{
	if (mPath->empty())
		return BTNodeState::Failure;


	// 다음 경로까지의 벡터
	Vec3 nextPos = (mPath->top() - GetOwner()->GetTransform()->GetPosition()).xz();

	// 현재 복귀 상태라면 스피드를 올린다.
	float speed{};
	//if (mReturnParam->val.b) {
	//	speed = mReturnSpeed;
	//	//mWalkMotion->SetSpeed(1.7f);
	//}
	//else {
	//	speed = mMoveSpeed;
	//	//mWalkMotion->SetSpeed(1.f);
	//}

	// 다음 경로를 향해 이동 및 회전
	GetOwner()->GetTransform()->RotateTargetAxisY(mPath->top(), mStat->GetStat_RotationSpeed());
	GetOwner()->GetTransform()->Translate(XMVector3Normalize(nextPos), speed * DELTA_TIME);

	// 다음 경로에 도착 시 해당 경로 삭제
	const float kMinDistance = 0.1f;
	if (nextPos.Length() < kMinDistance)
		mPath->pop();

	return BTNodeState::Success;
}



/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Move To Target  
/// __________________________________________________________________________

MonsterTask::MoveToTarget::MoveToTarget(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_MoveToTarget, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::MoveToTarget::~MoveToTarget()
{
	mEnemyController = nullptr;

}


BTNodeState MonsterTask::MoveToTarget::Evaluate()
{
	return BTNodeState();
}



/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Path Planning A*  
/// __________________________________________________________________________


MonsterTask::PathPlanning_AStar::PathPlanning_AStar(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_PathPlanningASatr, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::PathPlanning_AStar::~PathPlanning_AStar()
{
	mEnemyController = nullptr;

}


BTNodeState MonsterTask::PathPlanning_AStar::Evaluate()
{
	return BTNodeState();
}


bool MonsterTask::PathPlanning_AStar::PathPlanningAStar(Path::Pos start, Path::Pos dest)
{
	return true;
	
}

Path::Pos  MonsterTask::PathPlanning_AStar::FindNoneTileFromBFS(const Path::Pos& pos)
{
	/*std::queue<Pos> q;
	std::map<Pos, bool> visited;
	q.push(pos);

	Pos curPos{};
	while (!q.empty()) {
		curPos = q.front();
		q.pop();

		if (Scene::I->GetTileFromUniqueIndex(curPos) == Tile::None)
			return curPos;

		if (visited[curPos])
			continue;

		visited[curPos] = true;

		for (int dir = 0; dir < 4; ++dir) {
			Pos nextPos = curPos + gkFront[dir];
			q.push(nextPos);
		}
	}

	return curPos;*/
	return Path::Pos();

}



/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Path Planning To Spawn 
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToSapwn::Evaluate()
{
	return BTNodeState();
}

MonsterTask::PathPlanningToSapwn::PathPlanningToSapwn(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_PathPlanningToSpawn)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::PathPlanningToSapwn::~PathPlanningToSapwn()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Planning To Target  
/// __________________________________________________________________________

BTNodeState MonsterTask::PathPlanningToTarget::Evaluate()
{
	return BTNodeState();
}

MonsterTask::PathPlanningToTarget::PathPlanningToTarget(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_PathPlanningToTarget, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::PathPlanningToTarget::~PathPlanningToTarget()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Patrol 
/// __________________________________________________________________________

BTNodeState MonsterTask::Patrol::Evaluate()
{
	LOG_MGR->Cout("Patrol \n");

	return BTNodeState();
}

MonsterTask::Patrol::Patrol(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_Patrol, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::Patrol::~Patrol()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Patrol Range 
/// __________________________________________________________________________


BTNodeState MonsterTask::CheckPatrolRange::Evaluate()
{
	LOG_MGR->Cout("CheckPatrolRange \n");

	return BTNodeState();
}

MonsterTask::CheckPatrolRange::CheckPatrolRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckPatrolRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckPatrolRange::~CheckPatrolRange()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	return BTNodeState();
}

MonsterTask::CheckMindDetectionRange::CheckMindDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckMindDetectionRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckMindDetectionRange::~CheckMindDetectionRange()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDetectionRange::Evaluate()
{
	return BTNodeState();
}

MonsterTask::CheckDetectionRange::CheckDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDetectionRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckDetectionRange::~CheckDetectionRange()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Death  
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckDeath::Evaluate()
{
	LOG_MGR->Cout("CheckDeath\n");

	if (!mStat->IsDead())
		return BTNodeState::Failure;

	mEnemyController->SetState(EnemyInfo::State::Death);


	mAccTime += DELTA_TIME;

	ExecuteCallback();

	if (mAccTime >= mRemoveTime) {
		GetOwner()->DeActivate();
	}

	return BTNodeState::Success;
}

MonsterTask::CheckDeath::CheckDeath(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckDeath, callback)

{

	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckDeath::~CheckDeath()
{
	mEnemyController = nullptr;

}

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Check Attack Range 
/// __________________________________________________________________________

MonsterTask::CheckAttackRange::CheckAttackRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckAttackRange, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);
}

MonsterTask::CheckAttackRange::~CheckAttackRange()
{
	mEnemyController = nullptr;

}

BTNodeState MonsterTask::CheckAttackRange::Evaluate()
{
	LOG_MGR->Cout("CheckAttackRange\n");

	if (!mEnemyController->GetTargetObject()) {
		return BTNodeState::Failure;
	}

	if (mEnemyController->GetState() == EnemyInfo::State::Attack) {
		return BTNodeState::Success;
	}

	///* 타겟한 플레이어가 은신 상태라면.. */
	const auto& playerScript = mEnemyController->GetTargetObject()->GetScript<Script_Player>(ScriptInfo::Type::Stat);
	Skill* cloacking = playerScript->GetSkill(SkillInfo::Type::Cloaking);
	if (cloacking->GetState() == SkillInfo::State::Active) {
		mEnemyController->SetTargetObject(nullptr);
		return BTNodeState::Failure;
	}
	


	constexpr float minDistance = 1.f;
	const float distance = (GetOwner()->GetTransform()->GetPosition() - mEnemyController->GetTargetObject()->GetTransform()->GetPosition()).Length();
	if (distance < mStat->GetStat_AttackRange()) {

		Vec3		 TargetPos         = mEnemyController->GetTargetObject()->GetTransform()->GetPosition();
		Vec3		 MyPos             = GetOwner()->GetTransform()->GetPosition();
		const Vec3	 toTargetDir       = Vector3::Normalized(TargetPos - MyPos);

		const float	 angle      = Vector3::Angle(GetOwner()->GetTransform()->GetLook(), toTargetDir);
		if (minDistance < 1.f || angle < 80.f) {
			mEnemyController->SetState(EnemyInfo::State::Attack);
			return BTNodeState::Success;
		}
	}

	return BTNodeState::Failure;
}


