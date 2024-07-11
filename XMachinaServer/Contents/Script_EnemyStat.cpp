#include "pch.h"
#include "Script_EnemyStat.h"

Script_EnemyStat::Script_EnemyStat()
{
}

Script_EnemyStat::Script_EnemyStat(UINT32 id)
	: Script_Stat(id)
{
}

Script_EnemyStat::~Script_EnemyStat()
{
}

void Script_EnemyStat::Activate()
{
	Script::Activate();

}

void Script_EnemyStat::DeActivate()
{
	Script::DeActivate();

}

bool Script_EnemyStat::WakeUp()
{
	return false;
}

bool Script_EnemyStat::Start()
{
	return false;
}

bool Script_EnemyStat::Update()
{
	return false;
}

void Script_EnemyStat::OnDestroy()
{
}

void Script_EnemyStat::Attack()
{
}

void Script_EnemyStat::AttackCallback()
{
}

void Script_EnemyStat::Dead()
{
}



bool Script_EnemyStat::Hit(float damage, SPtr_GameObject instigator)
{
	return false;
}
