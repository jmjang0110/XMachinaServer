#include "pch.h"
#include "BTTask.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ▶▶▶ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	// 가장 가까운 적을 타겟으로 설정
	if (!SetTargetNearestEnemy()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Running;
	}

	// 경로 길찾기가 실행중이거나 감지 범위 내에 들어온 경우 다음 노드로 진행
	Vec3 MyPos = GetOwner()->GetTransform()->GetPosition();
	Vec3 TargetPos{};
	if (mEnemyController->IsMindControlled() == false) {
		TargetPos = mEnemyController->GetTargetPlayer()->GetTransform()->GetSnapShot().GetPosition();
	}
	else {
		TargetPos = mEnemyController->GetTargetMonster()->GetTransform()->GetSnapShot().GetPosition();
	}

	float dist_My_Target = (MyPos - TargetPos).Length();
	if (dist_My_Target < mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Success;
	}
	else {
		if (mEnemyController->IsMindControlled() == false) {
			mEnemyController->SetTargetPlayer(nullptr);
		}
		else{
			mEnemyController->SetTargetMonster(nullptr);
		}
	}

	return BTNodeState::Failure;
}

bool MonsterTask::CheckMindDetectionRange::SetTargetNearestEnemy()
{

	//if (mEnemyMgr->mTarget) {
	//	return true;
	//}

	//bool isSetTarget = false;
	//float minDistance = FLT_MAX;
	//const std::vector<sptr<Grid>>& grids = Scene::I->GetNeighborGrids(Scene::I->GetGridIndexFromPos(mObject->GetPosition()), true);
	//for (const auto& grid : grids) {
	//	for (const auto& object : grid->GetObjectsFromTag(ObjectTag::Enemy)) {
	//		// 해당 오브젝트가 자신이면 넘어감
	//		if (object == mObject) {
	//			continue;
	//		}

	//		// 해당 오브젝트가 죽었다면 넘어감
	//		if (object->GetComponent<Script_LiveObject>()->IsDead()) {
	//			continue;
	//		}

	//		float distance = Vec3::Distance(mObject->GetPosition(), object->GetPosition());
	//		if (distance < minDistance && distance < mEnemyMgr->mStat.DetectionRange) {
	//			minDistance = distance;
	//			mEnemyMgr->mTarget = object;
	//			isSetTarget = true;
	//		}
	//	}
	//}

	//return isSetTarget;
	return true;

}



MonsterTask::CheckMindDetectionRange::CheckMindDetectionRange(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_CheckMindDetectionRange, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);
}

MonsterTask::CheckMindDetectionRange::~CheckMindDetectionRange()
{
	mEnemyController = nullptr;

}

