#include "pch.h"
#include "Script_EnemyController.h"
#include "Script_Stat.h"
#include "NPCController.h"
#include "GameRoom.h"


Script_EnemyController::Script_EnemyController()
	: Script()
{
}

Script_EnemyController::Script_EnemyController(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script(owner, type, static_cast<UINT32>(type))
{
}

Script_EnemyController::~Script_EnemyController()
{

}

void Script_EnemyController::RemoveAllAnimation()
{

	GetOwner()->GetAnimation()->GetController()->SetValue("Walk", false);
	GetOwner()->GetAnimation()->GetController()->SetValue("Attack", false);
	GetOwner()->GetAnimation()->GetController()->SetValue("Return", false);
	GetOwner()->GetAnimation()->GetController()->SetValue("GetHit", false);

}

void Script_EnemyController::SetOwnerMonster(SPtr<GameMonster> ownerMonster)
{
	mOwnerMonster = ownerMonster;
	mOwnerNPCController = mOwnerMonster->GetOwnerNPCController();
}

void Script_EnemyController::Clone(SPtr<Component> other) 
{
	Script::Clone(other);
	SPtr<Script_EnemyController> otherScript = std::static_pointer_cast<Script_EnemyController>(other);

	const auto& ownerMon = std::static_pointer_cast<GameMonster>(GetOwner());
	SetOwnerMonster(ownerMon);
	ownerMon->SetEnemyController(this);
}

void Script_EnemyController::Activate()
{
	Script::Activate();

}

void Script_EnemyController::DeActivate()
{
	Script::DeActivate();

}

bool Script_EnemyController::WakeUp()
{
	return true;
}

bool Script_EnemyController::Start()
{
	Script::Start();

	mOwnerNPCController = mOwnerMonster->GetOwnerNPCController();
	mOwnerRoom          = mOwnerNPCController->GetOwnerRoom().get();

	return true;
}

bool Script_EnemyController::Update()
{
	// [BSH] : 이 부분에서 계속 업데이트를 하며 타겟을 없애기 때문에 문제가 생김. 타겟은 행동트리에서만 바꿀 것.
	//if (mTarget) {
	//	if (mTarget->GetScript<Script_Stat>(ScriptInfo::Type::Stat)){
	//		mTarget = nullptr;
	//	}
	//}
	return true;
}

void Script_EnemyController::OnDestroy()
{
}

void Script_EnemyController::Reset()
{
	mState = EnemyInfo::State::Idle;
	mTarget = nullptr;
	mPrevBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_END;
	mCurrBTType = FBProtocol::MONSTER_BT_TYPE::MONSTER_BT_TYPE_END;

	while (!mPaths.empty()) {
		mPaths.pop();
	};
}