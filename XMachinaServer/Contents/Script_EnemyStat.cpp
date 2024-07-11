#include "pch.h"
#include "Script_EnemyStat.h"

Script_EnemyStat::Script_EnemyStat()
{
}

Script_EnemyStat::Script_EnemyStat(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Stat(owner , type)
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
	Script::WakeUp();

	return false;
}

bool Script_EnemyStat::Start()
{
	Script::Start();

	return false;
}

bool Script_EnemyStat::Update()
{
	Script::Update();

	return false;
}

void Script_EnemyStat::OnDestroy()
{
	Script::OnDestroy();

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
