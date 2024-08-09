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

