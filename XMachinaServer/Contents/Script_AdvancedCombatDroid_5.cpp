#include "pch.h"
#include "Script_AdvancedCombatDroid_5.h"

Script_AdvancedCombatDroid_5::Script_AdvancedCombatDroid_5()
{
}

Script_AdvancedCombatDroid_5::Script_AdvancedCombatDroid_5(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
}

Script_AdvancedCombatDroid_5::~Script_AdvancedCombatDroid_5()
{
}

void Script_AdvancedCombatDroid_5::Clone(SPtr<Component> other) 
{
    Script_Enemy::Clone(other);
    SPtr<Script_AdvancedCombatDroid_5> otherScript = std::static_pointer_cast<Script_AdvancedCombatDroid_5>(other);

}

void Script_AdvancedCombatDroid_5::Activate()
{
}

void Script_AdvancedCombatDroid_5::DeActivate()
{
}

bool Script_AdvancedCombatDroid_5::WakeUp()
{
    return false;
}

bool Script_AdvancedCombatDroid_5::Start()
{
    return false;
}

bool Script_AdvancedCombatDroid_5::Update()
{
    return false;
}

void Script_AdvancedCombatDroid_5::OnDestroy()
{
}

void Script_AdvancedCombatDroid_5::Attack()
{
}

void Script_AdvancedCombatDroid_5::AttackCallback()
{
}

void Script_AdvancedCombatDroid_5::Dead()
{
}

bool Script_AdvancedCombatDroid_5::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
