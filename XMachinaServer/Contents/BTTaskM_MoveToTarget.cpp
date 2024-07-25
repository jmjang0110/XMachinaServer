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
#include "NPCController.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"

#include "ServerLib/ThreadManager.h"

/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Move To Target  
/// __________________________________________________________________________

MonsterTask::MoveToTarget::MoveToTarget(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_MoveToTarget, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

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
	bool IsMindControlled = mEnemyController->IsMindControlled();

	if (IsMindControlled == false) {
		if (!mEnemyController->GetTargetPlayer())
			return BTNodeState::Failure;
	}
	else {
		if (!mEnemyController->GetTargetMonster())
			return BTNodeState::Failure;
	}

	if (IsMindControlled == false) {
		mEnemyController->SetPathTargetObject(std::dynamic_pointer_cast<GameObject>(mEnemyController->GetTargetPlayer()));
	}
	else {
		mEnemyController->SetPathTargetObject(std::dynamic_pointer_cast<GameObject>(mEnemyController->GetTargetMonster()));
	}

	// 허리 쪽부터 광선을 쏴야 맞는다.
	Vec3 MyPos = GetOwner()->GetTransform()->GetPosition() + GetOwner()->GetTransform()->GetUp() * 0.5f;
	Vec3 TargetPos;
	Coordinate Target_SectorIdx;
	Coordinate My_SectorIdx;


	if (IsMindControlled == false) {
		TransformSnapShot snapShot = mEnemyController->GetTargetPlayer()->GetTransform()->GetSnapShot();
		TargetPos = snapShot.GetPosition() + snapShot.GetUp() * 0.5f;

	}
	else {
		TransformSnapShot snapShot = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot();
		TargetPos = snapShot.GetPosition() + snapShot.GetUp() * 0.5f;
	}


	// 오브젝트로부터 타겟까지의 벡터
	Vec3 toTarget = TargetPos - MyPos;

	// 타겟으로부터 오브젝트로 광선을 쏜다.
	Ray r{ MyPos, XMVector3Normalize(toTarget) };

	// 몬스터가 있는 섹터 인덱스
	/* A */ My_SectorIdx     = SectorController::GetSectorIdxByPosition(GetOwner()->GetTransform()->GetPosition());
	/* B */ Target_SectorIdx = SectorController::GetSectorIdxByPosition(TargetPos);
	/* C */ Coordinate Alpha_SectorIdx;

	std::vector<Coordinate> checkSectors{};
	checkSectors.push_back(My_SectorIdx);
	checkSectors.push_back(Target_SectorIdx);

	if (My_SectorIdx.z != Target_SectorIdx.z && My_SectorIdx.x != Target_SectorIdx.x) {
		
		Coordinate RT_sectorIDx = Coordinate(std::max(My_SectorIdx.x, Target_SectorIdx.x),std::max(My_SectorIdx.z, Target_SectorIdx.z));
		Coordinate Center       = SectorController::GetSectorStartPos(RT_sectorIDx);

		bool IsMyX_Positive    = (MyPos.x - Center.x) > 0;
		bool IsXInter_Positive = isXInterceptPositive(TargetPos, GetOwner()->GetTransform()->GetPosition()); // x 절편이 양수인지
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
		checkSectors.push_back(Alpha_SectorIdx);

	}
	
	// 타겟의 섹터 인덱스
	// 해당 광선에 맞은 다른 Static 오브젝트의 거리가 타겟까지의 거리보다 가까운 경우 벽에 막혀있는 경우이다.


	// 타겟이 속한 모든 그리드를 검사해야 한다.
	//SPtr_GameObject Target = mEnemyController->GetTargetObject();
	for (int i = 0; i < checkSectors.size(); ++i) {
		SectorController* SC = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetSectorController();
		if(SC->CollideCheckRay_MinimumDist(checkSectors[i], r, GameObjectInfo::Type::Building) < toTarget.Length()) { // Ray와 섹터의 빌딩들과 Ray 체크후 가장 짧은 길이로 비교 
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
		GetOwner()->GetAnimation()->GetController()->SetValue("Return", false);

		if (IsMindControlled == false) {
			GetOwner()->GetTransform()->RotateTargetAxisY(mEnemyController->GetTargetPlayer()->GetTransform()->GetSnapShot().GetPosition(), mStat->GetStat_RotationSpeed());
			float moveSpeed = mStat->GetStat_MoveSpeed();
			GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), moveSpeed * DELTA_TIME);
		}
		else {
			GetOwner()->GetTransform()->RotateTargetAxisY(mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetPosition(), mStat->GetStat_RotationSpeed());
			GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed()* DELTA_TIME);

		}
		Vec3 pos = GetOwner()->GetTransform()->GetPosition();
		mEnemyController->GetOwnerMonster()->SetPosition(pos); /* Snap Shot - Pos */
	}

	Vec3 pos = GetOwner()->GetTransform()->GetPosition();
	//LOG_MGR->Cout("[ ", GetOwner()->GetID(), " ] : ", pos.x, " ", pos.y, " ", pos.z, "\n");


	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET);;
	return BTNodeState::Success;
}

