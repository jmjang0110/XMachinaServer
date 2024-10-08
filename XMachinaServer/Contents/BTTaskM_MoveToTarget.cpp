#include "pch.h"
#include "BTTaskM_MoveToTarget.h"
#include "BTNode.h"
#include "BTTask.h"

#include "GameRoom.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"


#include "PlayerController.h"
#include "SectorController.h"
#include "NPCController.h"

#include "Script_Player.h"
#include "Script_EnemyController.h"
#include "Script_Enemy.h"

#include "FBsPacketFactory.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "RoomManager.h"

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Move To Target  
/// __________________________________________________________________________

MonsterTask::MoveToTarget::MoveToTarget(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_MoveToTarget, callback)
{
}

MonsterTask::MoveToTarget::~MoveToTarget()
{
	mEnemyController = nullptr;

}

bool MonsterTask::MoveToTarget::isXInterceptPositive(const Vec3& To, const Vec3& From)
{
	// 오브젝트로부터 타겟까지의 벡터
	Vec3 toTarget = To - From;
	if (toTarget.x == 0) {
		return false;
	}

	double m = To.z / To.x;
	double c = From.z - m * From.x;

	// x절편은 y = 0일 때, x = -c / m
	double xIntercept = -c / m;

	return xIntercept > 0;
}


BTNodeState MonsterTask::MoveToTarget::Evaluate()
{
	SPtr<GameObject> target = mEnemyController->GetTarget();
	if (!target) {
		return BTNodeState::Failure;
	}

	// 1. Set Path Target 
	mEnemyController->SetPathTargetObject(target);
	
	Vec3 Pos			                 = mOwner->GetTransform()->GetPosition();
	TransformSnapShot targetTansSnapShot = target->GetTransform()->GetSnapShot();
	Vec3 targetPos		                 = targetTansSnapShot.GetPosition();

	Vec3 objectAdjPos	= Pos + mOwner->GetTransform()->GetUp() * 0.5f;
	Vec3 targetAdjPos	= targetTansSnapShot.GetPosition() + targetTansSnapShot.GetUp() * 0.5f;
	MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);

	// 오브젝트로부터 타겟까지의 벡터
	Vec3 toTarget = targetAdjPos - objectAdjPos;

	// 타겟으로부터 오브젝트로 광선을 쏜다.
	Ray r{ objectAdjPos, XMVector3Normalize(toTarget) };


	// 몬스터가 있는 섹터 인덱스
	/* A */Coordinate My_SectorIdx = SectorController::GetSectorIdxByPosition(mOwner->GetTransform()->GetPosition());
	/* B */Coordinate Target_SectorIdx = SectorController::GetSectorIdxByPosition(targetPos);
	/* C */ Coordinate Alpha_SectorIdx;

	std::vector<Coordinate> checkSectors{};
	if(My_SectorIdx.checkNegative() == false)
		checkSectors.push_back(My_SectorIdx);
	if (Target_SectorIdx.checkNegative() == false)
		checkSectors.push_back(Target_SectorIdx);

	if (My_SectorIdx.z != Target_SectorIdx.z && My_SectorIdx.x != Target_SectorIdx.x) {

		Coordinate RT_sectorIDx = Coordinate(std::max(My_SectorIdx.x, Target_SectorIdx.x), std::max(My_SectorIdx.z, Target_SectorIdx.z));
		Coordinate Center       = SectorController::GetSectorStartPos(RT_sectorIDx);

		bool IsMyX_Positive    = (Pos.x - Center.x) > 0;
		bool IsXInter_Positive = isXInterceptPositive(targetPos, mOwner->GetTransform()->GetPosition()); // x 절편이 양수인지
		
		if (IsMyX_Positive == false /* x : 음수 */) {

			if (IsXInter_Positive == false)
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x, My_SectorIdx.z + 1);
			else
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x + 1, My_SectorIdx.z);
		}
		else { /* x : 양수 */

			if (IsXInter_Positive == false)
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x - 1, My_SectorIdx.z);
			else
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x, My_SectorIdx.z + 1);
		}
		if (Alpha_SectorIdx.checkNegative() == false)
			checkSectors.push_back(Alpha_SectorIdx);

	}

	// 타겟이 속한 모든 그리드를 검사해야 한다.
	for (int i = 0; i < checkSectors.size(); ++i) {
		SectorController* SC = mEnemyController->GetOwnerRoom()->GetSectorController();
		if (SC->CollideCheckRay_MinimumDist(checkSectors[i], r) < toTarget.Length()) { // Ray와 섹터의 빌딩들과 Ray 체크후 가장 짧은 길이로 비교 
			return BTNodeState::Failure;
		}
	}

	// 한 번이라도 장애물 없이 직선 경로라면 길찾기 경로 초기화
	while (!mEnemyController->GetPaths()->empty())
		mEnemyController->GetPaths()->pop();

	// 오브젝트로부터 타겟까지의 벡터
	const float kMinDistance = 0.1f;

	const float s = mStat->GetStat_MoveSpeed();
	const float d = mOwner->DeltaTime();


	// 타겟에 도착하지 않았을 경우에만 이동
	if (toTarget.Length() > kMinDistance) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Return", false);

		//mStat->GetStat_RotationSpeed();
		MonsterBTTask::mTransform->RotateTargetAxisY(targetTansSnapShot.GetPosition(), 500.f);
		MonsterBTTask::mTransform->Translate(mOwner->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed() * mOwner->DeltaTime());
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET);
	return BTNodeState::Success;
}

