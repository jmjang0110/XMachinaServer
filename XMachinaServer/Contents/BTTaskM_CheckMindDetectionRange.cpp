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
///	> ▶▶▶ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	// 가장 가까운 적을 타겟으로 설정
	if (!SetTargetNearestEnemy()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		GetOwner()->GetAnimation()->GetController()->SetValue("Walk", true);
		return BTNodeState::Running;
	}

	// 경로 길찾기가 실행중이거나 감지 범위 내에 들어온 경우 다음 노드로 진행
	Vec3 pos = GetOwner()->GetTransform()->GetPosition();
	Vec3 targetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();

	float dist = (pos - targetPos).Length();
	if (dist < mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		GetOwner()->GetAnimation()->GetController()->SetValue("Walk", true);
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

	bool isSetTarget = false;
	for (int i = 0; i < vList.VL_Monsters.size(); ++i) {
		if (vList.VL_Monsters[i]->IsActive() == false)
			continue;

		Vec3 enemyPos = vList.VL_Monsters[i]->GetTransform()->GetSnapShot().GetPosition();
		float distance = Vec3::Distance(pos, enemyPos);

		if (distance < minDistance && distance < mStat->GetStat_DetectionRange()) {
			minDistance = distance;
			target      = vList.VL_Monsters[i];
			isSetTarget = true;
		}
	}

	mEnemyController->SetTarget(target);

	return isSetTarget;

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

