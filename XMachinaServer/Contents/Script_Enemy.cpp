#include "pch.h"
#include "Script_Enemy.h"
#include "Script_EnemyStat.h"
#include "GameObject.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"
#include "Script_EnemyStat.h"
#include "Script_DefaultEnemyBT.h"
#include "Script_Phero.h"
#include "Script_PheroDropper.h"


Script_Enemy::Script_Enemy()
{
}

Script_Enemy::~Script_Enemy()
{
}

void Script_Enemy::Activate()
{
	Script::Activate();

}

void Script_Enemy::DeActivate()
{
	Script::DeActivate();

}


bool Script_Enemy::WakeUp()
{
	Script::WakeUp();


	mEnemyController = std::dynamic_pointer_cast<Script_EnemyController>(GetOwner()->AddScript<Script_EnemyController>(ScriptInfo::Type::EnemyController));
	GetOwner()->AddScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
	GetOwner()->AddScript<Script_DefaultEnemyBT>(ScriptInfo::Type::BehaviorTree);


    return true;
}

bool Script_Enemy::Start()
{

    return true;
}

bool Script_Enemy::Update()
{
	Script::Update();


	return true;
}

void Script_Enemy::OnDestroy()
{
	Script::OnDestroy();

}

void Script_Enemy::Attack()
{
	Script_EnemyStat::Attack();

}

void Script_Enemy::AttackCallback()
{
}

void Script_Enemy::Dead()
{
	Script_EnemyStat::Dead();

}

bool Script_Enemy::Hit(float damage, SPtr_GameObject instigator)
{

	bool res = Script_EnemyStat::Hit(damage, instigator);


	if (nullptr != instigator) {
		mEnemyController->SetTargetObject(instigator);
	}

	return res;
}

