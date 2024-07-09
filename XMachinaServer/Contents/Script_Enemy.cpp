#include "pch.h"
#include "Script_Enemy.h"
#include "Script_EnemyStat.h"
#include "GameObject.h"


bool Script_Enemy::WakeUp()
{
	Script::WakeUp();

    return true;
}

bool Script_Enemy::Start()
{

    return true;
}

bool Script_Enemy::Update()
{

    return true;
}

void Script_Enemy::OnDestroy()
{
}

void Script_Enemy::Attack()
{
}

void Script_Enemy::Dead()
{
}

bool Script_Enemy::Hit(float damage, SPtr_GameObject instigator)
{
	bool res = Script_EnemyStat::Hit(damage, instigator);


	if (nullptr != instigator) {
		mEnemyController->SetTargetObject(instigator);
	}

	return res;
}

