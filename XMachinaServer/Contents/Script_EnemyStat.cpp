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

void Script_EnemyStat::Attack()
{
}

void Script_EnemyStat::Death()
{
}

bool Script_EnemyStat::Hit(float damage, GameObject* instigator)
{
	return false;
}
