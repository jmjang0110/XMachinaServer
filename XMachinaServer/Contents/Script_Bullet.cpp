#include "pch.h"
#include "Script_Bullet.h"

Script_Bullet::Script_Bullet()
{
}

Script_Bullet::Script_Bullet(SPtr<GameObject> owner, ScriptInfo::Type type)
{
}

Script_Bullet::~Script_Bullet()
{
}

void Script_Bullet::Clone(SPtr<Component> other)
{
    Script::Clone(other);

}

void Script_Bullet::Activate()
{
    Script::Activate();

}

void Script_Bullet::DeActivate()
{
    Script::DeActivate();

}

bool Script_Bullet::WakeUp()
{
    Script::WakeUp();

    return false;
}

bool Script_Bullet::Start()
{
    Script::Start();

    return false;
}

bool Script_Bullet::Update()
{
    Script::Update();

    return false;
}

void Script_Bullet::OnDestroy()
{
    Script::OnDestroy();

}
