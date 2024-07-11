#include "pch.h"
#include "Script_EnemyController.h"
#include "Script_Stat.h"



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
	return true;
}

bool Script_EnemyController::Update()
{
	if (mTarget) {
		if (mTarget->GetScript<Script_Stat>(ScriptInfo::Type::Stat)){
			mTarget = nullptr;
		}
	}
	return true;
}

void Script_EnemyController::OnDestroy()
{
}

void Script_EnemyController::Reset()
{
	mState = EnemyInfo::State::Idle;
	mTarget = nullptr;

	while (!mPaths.empty()) {
		mPaths.pop();
	};
}