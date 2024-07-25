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
///	> ������ Task Move To Target  
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

	// �㸮 �ʺ��� ������ ���� �´´�.
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


	// ������Ʈ�κ��� Ÿ�ٱ����� ����
	Vec3 toTarget = TargetPos - MyPos;

	// Ÿ�����κ��� ������Ʈ�� ������ ���.
	Ray r{ MyPos, XMVector3Normalize(toTarget) };

	// ���Ͱ� �ִ� ���� �ε���
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
		bool IsXInter_Positive = isXInterceptPositive(TargetPos, GetOwner()->GetTransform()->GetPosition()); // x ������ �������
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
	
	// Ÿ���� ���� �ε���
	// �ش� ������ ���� �ٸ� Static ������Ʈ�� �Ÿ��� Ÿ�ٱ����� �Ÿ����� ����� ��� ���� �����ִ� ����̴�.


	// Ÿ���� ���� ��� �׸��带 �˻��ؾ� �Ѵ�.
	//SPtr_GameObject Target = mEnemyController->GetTargetObject();
	for (int i = 0; i < checkSectors.size(); ++i) {
		SectorController* SC = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetSectorController();
		if(SC->CollideCheckRay_MinimumDist(checkSectors[i], r, GameObjectInfo::Type::Building) < toTarget.Length()) { // Ray�� ������ ������� Ray üũ�� ���� ª�� ���̷� �� 
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

