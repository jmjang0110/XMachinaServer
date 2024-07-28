#include "pch.h"
#include "BTTaskM_MoveToTarget.h"

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

#include "FBsPacketFactory.h"
#include "GameManager.h"
#include "GameRoom.h"
#include "GameObject.h"


/// +-------------------------------------------------------------------------
///	> ������ Task Move To Target  
/// __________________________________________________________________________

MonsterTask::MoveToTarget::MoveToTarget(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_MoveToTarget, callback)
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
	SPtr<GameObject> target = mEnemyController->GetTarget();
	if (!target) {
		return BTNodeState::Failure;
	}

	// 1. Set Path Target 
	mEnemyController->SetPathTargetObject(target);

	
	Vec3 Pos			                 = GetOwner()->GetTransform()->GetPosition();
	TransformSnapShot targetTansSnapShot = target->GetTransform()->GetSnapShot();
	Vec3 targetPos		                 = targetTansSnapShot.GetPosition();

	Vec3 objectAdjPos	= Pos + GetOwner()->GetTransform()->GetUp() * 0.5f;
	Vec3 targetAdjPos	= targetTansSnapShot.GetPosition() + targetTansSnapShot.GetUp() * 0.5f;

	// ������Ʈ�κ��� Ÿ�ٱ����� ����
	Vec3 toTarget = targetAdjPos - objectAdjPos;

	// Ÿ�����κ��� ������Ʈ�� ������ ���.
	Ray r{ objectAdjPos, XMVector3Normalize(toTarget) };


	// ���Ͱ� �ִ� ���� �ε���
	/* A */Coordinate My_SectorIdx = SectorController::GetSectorIdxByPosition(GetOwner()->GetTransform()->GetPosition());
	/* B */Coordinate Target_SectorIdx = SectorController::GetSectorIdxByPosition(targetPos);
	/* C */ Coordinate Alpha_SectorIdx;

	std::vector<Coordinate> checkSectors{};
	if(My_SectorIdx.checkNegative() == false)
		checkSectors.push_back(My_SectorIdx);
	if (Target_SectorIdx.checkNegative() == false)
		checkSectors.push_back(Target_SectorIdx);

	if (My_SectorIdx.z != Target_SectorIdx.z && My_SectorIdx.x != Target_SectorIdx.x) {

		Coordinate RT_sectorIDx = Coordinate(std::max(My_SectorIdx.x, Target_SectorIdx.x), std::max(My_SectorIdx.z, Target_SectorIdx.z));
		Coordinate Center = SectorController::GetSectorStartPos(RT_sectorIDx);

		bool IsMyX_Positive = (Pos.x - Center.x) > 0;
		bool IsXInter_Positive = isXInterceptPositive(targetPos, GetOwner()->GetTransform()->GetPosition()); // x ������ �������
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
		if (Alpha_SectorIdx.checkNegative() == false)
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

		GetOwner()->GetTransform()->RotateTargetAxisY(targetTansSnapShot.GetPosition(), mStat->GetStat_RotationSpeed());
		GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed() * GetOwner()->GetDeltaTime());
		
		//Vec3 Pos = GetOwner()->GetTransform()->GetPosition();
		//Vec3 Rot = Quaternion::ToEuler(GetOwner()->GetTransform()->GetRotation());
		//auto spkt = FBS_FACTORY->SPkt_Monster_Transform(GetOwner()->GetID(), Pos, Rot);
		//GAME_MGR->BroadcastAllRoom(spkt);
	}


	mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET);;
	mEnemyController->GetOwnerMonster()->SetBTState(FBProtocol::MONSTER_BT_TYPE_MOVE_TO_TARGET);

	return BTNodeState::Success;

}

