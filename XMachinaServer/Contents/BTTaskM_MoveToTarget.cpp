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
///	> ������ Task Move To Target  
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

	// �㸮 �ʺ��� ������ ���� �´´�.
	Vec3 objectAdjPos = GetOwner()->GetTransform()->GetPosition() + GetOwner()->GetTransform()->GetUp() * 0.5f;
	Vec3 targetAdjPos = mEnemyController->GetTargetObject()->GetTransform()->GetPosition()
		+ mEnemyController->GetTargetObject()->GetTransform()->GetUp() * 0.5f;

	// ������Ʈ�κ��� Ÿ�ٱ����� ����
	Vec3 toTarget = targetAdjPos - objectAdjPos;

	// Ÿ�����κ��� ������Ʈ�� ������ ���.
	Ray r{ objectAdjPos, XMVector3Normalize(toTarget) };

	// Ÿ���� ���� ��� �׸��带 �˻��ؾ� �Ѵ�.
	SPtr_GameObject Target = mEnemyController->GetTargetObject();
	
	// �ش� ������ ���� �ٸ� Static ������Ʈ�� �Ÿ��� Ÿ�ٱ����� �Ÿ����� ����� ��� ���� �����ִ� ����̴�.

	Coordinate M_SectorIdx = SectorController::GetSectorIdxByPosition(GetOwner()->GetTransform()->GetPosition());

	if (Target) {
		GameObjectInfo::Type type = Target->GetType();
		if (GameObjectInfo::Type::GamePlayer == type) {
			
			SPtr<GamePlayer> gamePlayer  = std::dynamic_pointer_cast<GamePlayer>(Target);
			Vec3			 PlayerPos   = gamePlayer->GetTransform()->GetPosition();
			Coordinate		 P_SectorIdx = SectorController::GetSectorIdxByPosition(PlayerPos);

			M_SectorIdx;
			P_SectorIdx;

			// ������ ������ ���Ѵ�.
			// ���߿�....
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

		GetOwner()->GetTransform()->RotateTargetAxisY(mEnemyController->GetTargetObject()->GetTransform()->GetPosition(), mStat->GetStat_RotationSpeed());
		GetOwner()->GetTransform()->Translate(GetOwner()->GetTransform()->GetLook(), mStat->GetStat_MoveSpeed() * DELTA_TIME);
	}

	return BTNodeState::Success;
}

