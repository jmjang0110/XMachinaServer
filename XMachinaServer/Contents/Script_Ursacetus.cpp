#include "pch.h"
#include "Script_Ursacetus.h"
#include "GameObject.h"
Script_Ursacetus::Script_Ursacetus()
    : Script_Enemy()

{
}

Script_Ursacetus::Script_Ursacetus(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script_Enemy(owner,  type)
{

}

Script_Ursacetus::~Script_Ursacetus()
{
}

SPtr<Component> Script_Ursacetus::Clone(SPtr<GameObject> copyOwner) const
{
    return SPtr<Component>();
}

void Script_Ursacetus::Activate()
{
}

void Script_Ursacetus::DeActivate()
{
}

bool Script_Ursacetus::WakeUp()
{
    Script_Enemy::WakeUp();

    return false;
}

bool Script_Ursacetus::Start()
{
    Script_Enemy::Start();

    return false;
}

bool Script_Ursacetus::Update()
{
    Script_Enemy::Update();

    return true;
}

void Script_Ursacetus::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Ursacetus::Attack()
{
    Script_Enemy::Attack();

}

void Script_Ursacetus::AttackCallback()
{
}

void Script_Ursacetus::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Ursacetus::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return false;
}
