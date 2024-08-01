#include "pch.h"
#include "Script_Player.h"
#include "GamePlayer.h"

Script_Player::Script_Player()
    : Script_PlayerStat()
{
}

Script_Player::Script_Player(SPtr<GamePlayer> owner, ScriptInfo::Type type)
    : Script_PlayerStat(owner, type)
{


}
Script_Player::Script_Player(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script_PlayerStat(owner, type)
{


}
Script_Player::~Script_Player()
{

}

void Script_Player::Clone(SPtr<Component> other) 
{
    Script_PlayerStat::Clone(other);
    SPtr<Script_Player> otherScript = std::static_pointer_cast<Script_Player>(other);

}

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

bool Script_Player::Attack()
{
    return true;
}

void Script_Player::Dead()
{
}

bool Script_Player::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
