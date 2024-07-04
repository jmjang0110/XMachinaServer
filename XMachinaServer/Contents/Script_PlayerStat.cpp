#include "pch.h"
#include "Script_PlayerStat.h"

Script_PlayerStat::Script_PlayerStat()
{

}

Script_PlayerStat::Script_PlayerStat(UINT32 id)
	: Script_Stat(id)
{

}

Script_PlayerStat::~Script_PlayerStat()
{
}

bool Script_PlayerStat::WakeUp()
{
	return false;
}

bool Script_PlayerStat::Start()
{
	return false;
}

bool Script_PlayerStat::Update()
{
	return false;
}

void Script_PlayerStat::Attack()
{
}

void Script_PlayerStat::Death()
{
}

bool Script_PlayerStat::Hit(float damage, GameObject* instigator)
{
	return false;
}
