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
	MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);

	// 가장 가까운 적을 타겟으로 설정
	SetTargetNearestEnemy();

	return BTNodeState::Success;

	//// 경로 길찾기가 실행중이거나 감지 범위 내에 들어온 경우 다음 노드로 진행
	//Vec3 pos = MonsterBTTask::mTransform->GetPosition();
	//Vec3 targetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();

	//float dist = (pos - targetPos).Length();
	//if (dist < mStat->GetStat_DetectionRange()) {
	//	MonsterBTTask::mAnimation->GetController()->SetValue("Walk", true);
	//	return BTNodeState::Success;
	//}
	//else {
	//	mEnemyController->SetTarget(nullptr);
	//}

	//return BTNodeState::Failure;
}

bool MonsterTask::CheckMindDetectionRange::SetTargetNearestEnemy()
{
	if (mEnemyController->GetTarget()) {
		return true;
	}

	float minDistance = std::numeric_limits<float>::max();

	Vec3				monsterPos       = MonsterBTTask::mTransform->GetPosition();
	ViewList			monsterVList     = mEnemyController->GetOwnerRoom()->GetSectorController()->GetViewList(monsterPos, mStat->GetStat_DetectionRange(), false);
	SPtr<GameObject>	target			 = nullptr;

	for (auto& iter : monsterVList.VL_Monsters) {
		if (iter.second->S_GetObjectState() == Script_Stat::ObjectState::Dead) {
			continue;
		}

		if (iter.second->GetID() == mEnemyController->GetOwnerMonster()->GetID()) {
			continue;
		}

		Vec3	enemyPos = iter.second->GetTransform()->GetSnapShot().GetPosition();
		float	distance = Vec3::Distance(monsterPos, enemyPos);

		if (distance < minDistance) {
			minDistance = distance;
			target      = iter.second;
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

