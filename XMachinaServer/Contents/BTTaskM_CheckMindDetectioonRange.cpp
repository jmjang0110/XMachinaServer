#include "pch.h"
#include "BTTask.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Mind Detection Range 
/// __________________________________________________________________________

BTNodeState MonsterTask::CheckMindDetectionRange::Evaluate()
{
	// ���� ����� ���� Ÿ������ ����
	if (!SetTargetNearestEnemy()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Running;
	}

	// ��� ��ã�Ⱑ �������̰ų� ���� ���� ���� ���� ��� ���� ���� ����
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
	//		// �ش� ������Ʈ�� �ڽ��̸� �Ѿ
	//		if (object == mObject) {
	//			continue;
	//		}

	//		// �ش� ������Ʈ�� �׾��ٸ� �Ѿ
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
	mStat = GetStat(owner->GetType());

}

MonsterTask::CheckMindDetectionRange::~CheckMindDetectionRange()
{
	mEnemyController = nullptr;

}

