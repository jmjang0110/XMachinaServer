#include "pch.h"
#include "Script_PlayerStat.h"

Script_PlayerStat::Script_PlayerStat()
{

}

Script_PlayerStat::Script_PlayerStat(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Stat(owner, type)
{

}

Script_PlayerStat::~Script_PlayerStat()
{
}

SPtr<Component> Script_PlayerStat::Clone(SPtr<GameObject> copyOwner) const
{
	return SPtr<Component>();
}

void Script_PlayerStat::Activate()
{
	Script::Activate();

}

void Script_PlayerStat::DeActivate()
{
	Script::DeActivate();

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

void Script_PlayerStat::OnDestory()
{
}

void Script_PlayerStat::Attack()
{

}

void Script_PlayerStat::Dead()
{
	Script_Stat::Dead();

}

bool Script_PlayerStat::Hit(float damage, SPtr_GameObject instigator)
{
	Script_Stat::Hit(damage, instigator);

	return false;
}

void Script_PlayerStat::SetSpawn(const Vec3& pos)
{
}



/// +-------------------------------------------
///	 >> Phero
/// -------------------------------------------+
void Script_PlayerStat::AddPheroAmount(float pheroAmount)
{
}

bool Script_PlayerStat::ReducePheroAmount(float pheroCost, bool checkOnly)
{
	return false;
}
