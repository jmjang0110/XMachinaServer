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

	// Ÿ���� ������ ��찡 �ƴ϶�� �÷��̾ ���󰣴�. 
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

	// �㸮 �ʺ��� ������ ���� �´´�.
	Vec3 objectAdjPos = pos + GetOwner()->GetTransform()->GetUp() * 0.5f;
	Vec3 targetAdjPos = targetTransSnapShot.GetPosition() + targetTransSnapShot.GetUp() * 0.5f;

	Vec3 toTarget = targetAdjPos - objectAdjPos;

	// Ÿ�����κ��� ������Ʈ�� ������ ���.
	Ray r{ objectAdjPos, XMVector3Normalize(toTarget) };



	// ���Ͱ� �ִ� ���� �ε���
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
		bool IsXInter_Positive = isXInterceptPositive(targetTransSnapShot.GetPosition(), GetOwner()->GetTransform()->GetPosition()); // x ������ �������
		if (IsMyX_Positive == false /* x : ���� */) {

			if (IsXInter_Positive == false)
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x, My_SectorIdx.z + 1);
			else
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x + 1, My_SectorIdx.z);
		}
		else { /* x : ��� */

			if (IsXInter_Positive == false)
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x - 1, My_SectorIdx.z);
			else
				Alpha_SectorIdx = Coordinate(My_SectorIdx.x, My_SectorIdx.z + 1);
		}
		checkSectors.push_back(Alpha_SectorIdx);

	}

	// Ÿ���� ���� ��� �׸��带 �˻��ؾ� �Ѵ�.
	for (int i = 0; i < checkSectors.size(); ++i) {
		SectorController* SC = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetSectorController();
		if (SC->CollideCheckRay_MinimumDist(checkSectors[i], r, GameObjectInfo::Type::Building) < toTarget.Length()) { // Ray�� ������ ������� Ray üũ�� ���� ª�� ���̷� �� 
			return BTNodeState::Failure;
		}
	}

	// �� ���̶� ��ֹ� ���� ���� ��ζ�� ��ã�� ��� �ʱ�ȭ
	while (!mEnemyController->GetPaths()->empty())
		mEnemyController->GetPaths()->pop();

	// ������Ʈ�κ��� Ÿ�ٱ����� ����
	const float kMinDistance = 0.1f;

	// Ÿ�ٿ� �������� �ʾ��� ��쿡�� �̵�
	if (toTarget.Length() > kMinDistance) {
		GetOwner()->GetAnimation()->GetController()->SetValue("Return", false);

		GetOwner()->GetTransform()->RotateTargetAxisY(targetTransSnapShot.GetPosition(), mStat->GetStat_RotationSpeed());
		GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed() * DELTA_TIME);
	}

	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET);;
	return BTNodeState::Success;
}

bool MonsterTask::BTTaskM_MoveToMindControlInvoker::isXInterceptPositive(const Vec3& To, const Vec3& From)
{
	// ������Ʈ�κ��� Ÿ�ٱ����� ����
	Vec3 toTarget = To - From;
	if (toTarget.x == 0) {
		return false;
	}

	double m = To.z / To.x;
	double c = From.z - m * From.x;

	// x������ y = 0�� ��, x = -c / m
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
