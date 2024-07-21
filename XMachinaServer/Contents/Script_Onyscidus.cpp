#include "pch.h"
#include "Script_Onyscidus.h"

Script_Onyscidus::Script_Onyscidus()
{
}

Script_Onyscidus::Script_Onyscidus(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Enemy(owner, type)
{
}

Script_Onyscidus::~Script_Onyscidus()
{
}

void Script_Onyscidus::Clone(SPtr<Component> other) 
{
	Script_Enemy::Clone(other);
	SPtr<Script_Onyscidus> otherScript = std::static_pointer_cast<Script_Onyscidus>(other);

}

void Script_Onyscidus::Activate()
{
}

void Script_Onyscidus::DeActivate()
{
}

bool Script_Onyscidus::WakeUp()
{
	return false;
}

bool Script_Onyscidus::Start()
{
	return false;
}

bool Script_Onyscidus::Update()
{
	return false;
}

void Script_Onyscidus::OnDestroy()
{
}

void Script_Onyscidus::Attack()
{
}

void Script_Onyscidus::AttackCallback()
{
}

void Script_Onyscidus::Dead()
{
}

bool Script_Onyscidus::Hit(float damage, SPtr_GameObject instigator)
{
	return false;
}
