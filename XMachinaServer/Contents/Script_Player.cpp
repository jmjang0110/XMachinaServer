#include "pch.h"
#include "Script_Player.h"

void Script_Player::Activate()
{
    Script_PlayerStat::Activate();

}

void Script_Player::DeActivate()
{
    Script_PlayerStat::DeActivate();

}

bool Script_Player::WakeUp()
{
    return false;
}

bool Script_Player::Start()
{
    return false;
}

bool Script_Player::Update()
{
    return false;
}

void Script_Player::OnDestroy()
{
}

void Script_Player::Attack()
{
}

void Script_Player::Dead()
{
}

bool Script_Player::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
