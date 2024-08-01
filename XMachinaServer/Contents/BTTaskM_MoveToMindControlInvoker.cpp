#include "pch.h"
#include "BTTaskM_MoveToMindControlInvoker.h"

#include "SectorController.h"
#include "NPCController.h"
#include "GameRoom.h"
#include "TimeManager.h"
#include "ServerLib/ThreadManager.h"


BTNodeState MonsterTask::BTTaskM_MoveToMindControlInvoker::Evaluate()
{
	SPtr<GameObject> target = mEnemyController->GetTarget();

	// 타겟이 몬스터인 경우가 아니라면 플레이어를 따라간다. 
	if (!target) {
		if (mInvoker) {
			target = mInvoker; // To Follow Invoker ( Player ) 
			mEnemyController->SetPathTargetObject(mInvoker);
		}
		else {
			return BTNodeState::Failure;
		}
	}

	Vec3 pos = GetOwner()->GetTransform()->GetPosition();
	TransformSnapShot targetTransSnapShot = target->GetTransform()->GetSnapShot();

	// 허리 쪽부터 광선을 쏴야 맞는다.
	Vec3 objectAdjPos = pos + GetOwner()->GetTransform()->GetUp() * 0.5f;
	Vec3 targetAdjPos = targetTransSnapShot.GetPosition() + targetTransSnapShot.GetUp() * 0.5f;

	Vec3 toTarget = targetAdjPos - objectAdjPos;

	// 타겟으로부터 오브젝트로 광선을 쏜다.
	Ray r{ objectAdjPos, XMVector3Normalize(toTarget) };



	// 몬스터가 있는 섹터 인덱스
	/* A */Coordinate My_SectorIdx = SectorController::GetSectorIdxByPosition(GetOwner()->GetTransform()->GetPosition());
	/* B */Coordinate Target_SectorIdx = SectorController::GetSectorIdxByPosition(targetTransSnapShot.GetPosition());
	/* C */ Coordinate Alpha_SectorIdx;

	std::vector<Coordinate> checkSectors{};
	checkSectors.push_back(My_SectorIdx);
	checkSectors.push_back(Target_SectorIdx);

	if (My_SectorIdx.z != Target_SectorIdx.z && My_SectorIdx.x != Target_SectorIdx.x) {

		Coordinate RT_sectorIDx = Coordinate(std::max(My_SectorIdx.x, Target_SectorIdx.x), std::max(My_SectorIdx.z, Target_SectorIdx.z));
		Coordinate Center = SectorController::GetSectorStartPos(RT_sectorIDx);

		bool IsMyX_Positive = (pos.x - Center.x) > 0;
		bool IsXInter_Positive = isXInterceptPositive(targetTransSnapShot.GetPosition(), GetOwner()->GetTransform()->GetPosition()); // x 절편이 양수인지
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

	// 타겟이 속한 모든 그리드를 검사해야 한다.
	for (int i = 0; i < checkSectors.size(); ++i) {
		SectorController* SC = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetSectorController();
		if (SC->CollideCheckRay_MinimumDist(checkSectors[i], r, GameObjectInfo::Type::Building) < toTarget.Length()) { // Ray와 섹터의 빌딩들과 Ray 체크후 가장 짧은 길이로 비교 
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

		GetOwner()->GetTransform()->RotateTargetAxisY(targetTransSnapShot.GetPosition(), mStat->GetStat_RotationSpeed());
		GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed() * GetOwner()->GetDeltaTime());
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET);;
	return BTNodeState::Success;
}

bool MonsterTask::BTTaskM_MoveToMindControlInvoker::isXInterceptPositive(const Vec3& To, const Vec3& From)
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

MonsterTask::BTTaskM_MoveToMindControlInvoker::BTTaskM_MoveToMindControlInvoker(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_MoveToMindControlInvoker, callback)

{
}

MonsterTask::BTTaskM_MoveToMindControlInvoker::~BTTaskM_MoveToMindControlInvoker()
{
}
