#include "pch.h"
#include "Script_EnemyController.h"

bool Script_EnemyController::WakeUp()
{
	return false;
}

bool Script_EnemyController::Start()
{
	return false;
}

bool Script_EnemyController::Update()
{
	return false;
}

void Script_EnemyController::Reset()
{
	mState = EnemyInfo::State::Idle;
	mTarget = nullptr;

	while (!mPaths.empty()) {
		mPaths.pop();
	};
}

Script_EnemyController::Script_EnemyController()
	: Script()
{
}

Script_EnemyController::Script_EnemyController(UINT32 id)
	: Script(id)
{
}

Script_EnemyController::~Script_EnemyController()
{
}
