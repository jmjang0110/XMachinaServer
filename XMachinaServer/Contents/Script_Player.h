#pragma once

#include "Script_PlayerStat.h"
#include "GameSkill.h"
#include "GameBullet.h"



class GamePlayer;
class Script_Player : public Script_PlayerStat
{
private:

public:  
	Script_Player();
	Script_Player(SPtr<GamePlayer> owner, ScriptInfo::Type type);
	Script_Player(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Player();


public:
	virtual void Clone(SPtr<Component> other) ;



};

