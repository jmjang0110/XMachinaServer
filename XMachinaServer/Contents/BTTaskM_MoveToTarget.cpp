#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
#include "GamePlayer.h"
#include "SectorController.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "GameRoom.h"

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
	if (!mEnemyController->GetTargetObject()) {
		return BTNodeState::Failure;
	}

	mEnemyController->SetPathTargetObject(mEnemyController->GetTargetObject());

	// 허리 쪽부터 광선을 쏴야 맞는다.
	Vec3 objectAdjPos = GetOwner()->GetTransform()->GetPosition() + GetOwner()->GetTransform()->GetUp() * 0.5f;
	Vec3 targetAdjPos = mEnemyController->GetTargetObject()->GetTransform()->GetPosition()
		+ mEnemyController->GetTargetObject()->GetTransform()->GetUp() * 0.5f;

	// 오브젝트로부터 타겟까지의 벡터
	Vec3 toTarget = targetAdjPos - objectAdjPos;

	// 타겟으로부터 오브젝트로 광선을 쏜다.
	Ray r{ objectAdjPos, XMVector3Normalize(toTarget) };

	// 타겟이 속한 모든 그리드를 검사해야 한다.
	SPtr_GameObject Target = mEnemyController->GetTargetObject();
	
	// 해당 광선에 맞은 다른 Static 오브젝트의 거리가 타겟까지의 거리보다 가까운 경우 벽에 막혀있는 경우이다.

	Coordinate M_SectorIdx = SectorController::GetSectorIdxByPosition(GetOwner()->GetTransform()->GetPosition());

	if (Target) {
		GameObjectInfo::Type type = Target->GetType();
		if (GameObjectInfo::Type::GamePlayer == type) {
			
			SPtr<GamePlayer> gamePlayer  = std::dynamic_pointer_cast<GamePlayer>(Target);
			Vec3			 PlayerPos   = gamePlayer->GetTransform()->GetPosition();
			Coordinate		 P_SectorIdx = SectorController::GetSectorIdxByPosition(PlayerPos);

			M_SectorIdx;
			P_SectorIdx;

			// 직선의 방정식 구한다.
			// 나중에....
			return BTNodeState::Failure;

		}
	}

	// 한 번이라도 장애물 없이 직선 경로라면 길찾기 경로 초기화
	while (!mEnemyController->GetPaths()->empty())
		mEnemyController->GetPaths()->pop();

	// 오브젝트로부터 타겟까지의 벡터
	const float kMinDistance = 0.1f;

	// 타겟에 도착하지 않았을 경우에만 이동
	if (toTarget.Length() > kMinDistance) {

		GetOwner()->GetTransform()->RotateTargetAxisY(mEnemyController->GetTargetObject()->GetTransform()->GetPosition(), mStat->GetStat_RotationSpeed());
		GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed() * DELTA_TIME);
	}

	return BTNodeState::Success;
}

