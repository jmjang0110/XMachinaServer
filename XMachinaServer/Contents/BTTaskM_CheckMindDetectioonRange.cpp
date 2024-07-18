#include "pch.h"
#include "BTTask.h"

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
	if ((GetOwner()->GetTransform()->GetPosition() 
		- mEnemyController->GetTargetObject()->GetTransform()->GetPosition()).Length() 
		< mStat->GetStat_DetectionRange()) {
		mEnemyController->SetState(EnemyInfo::State::Walk);
		return BTNodeState::Success;
	}
	else {
		mEnemyController->SetTargetObject(nullptr);
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
	mStat = GetOwner()->GetScript<Script_Enemy>(ScriptInfo::Type::Stat);

}

MonsterTask::CheckMindDetectionRange::~CheckMindDetectionRange()
{
	mEnemyController = nullptr;

}

