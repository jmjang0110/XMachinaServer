#include "pch.h"
#include "BTTaskM_CheckMindDetectionRange.h"

#include "BTTask.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"

#include "SectorController.h"
#include "NPCController.h"
#include "GameRoom.h"

/// +-------------------------------------------------------------------------
///	> ������ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	// ���� ����� ���� Ÿ������ ����
	if (!SetTargetNearestEnemy()) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);
		return BTNodeState::Running;
	}

	// ��� ��ã�Ⱑ �������̰ų� ���� ���� ���� ���� ��� ���� ���� ����
	Vec3 pos = MonsterBTTask::mTransform->GetPosition();
	Vec3 targetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();

	float dist = (pos - targetPos).Length();
	if (dist < mStat->GetStat_DetectionRange()) {
		MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);
		return BTNodeState::Success;
	}
	else {
		mEnemyController->SetTarget(nullptr);
	}

	return BTNodeState::Failure;
}

bool MonsterTask::CheckMindDetectionRange::SetTargetNearestEnemy()
{
	if (mEnemyController->GetTarget()) {
		return true;
	}

	float minDistance = std::numeric_limits<float>::max();

	Vec3 pos       = GetOwner()->GetTransform()->GetPosition();
	ViewList vList = mEnemyController->GetOwnerMonster()->GetOwnerNPCController()->GetOwnerRoom()->GetSectorController()->GetViewList(pos, 10.f);
	SPtr<GameObject> target = nullptr;

	for (int i = 0; i < vList.VL_Monsters.size(); ++i) {
		if (vList.VL_Monsters[i]->IsActive() == false)
			continue;

		Vec3 enemyPos = vList.VL_Monsters[i]->GetTransform()->GetSnapShot().GetPosition();
		float distance = Vec3::Distance(pos, enemyPos);

		if (distance < minDistance) {
			minDistance = distance;
			target      = vList.VL_Monsters[i];
		}
	}

	mEnemyController->SetTarget(target);

	return true;

}



MonsterTask::CheckMindDetectionRange::CheckMindDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_CheckMindDetectionRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	mStat = GetStat(owner->GetType());

}

MonsterTask::CheckMindDetectionRange::~CheckMindDetectionRange()
{
	mEnemyController = nullptr;

}

