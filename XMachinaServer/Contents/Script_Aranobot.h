#pragma once
#include "Script_Enemy.h"

class GameObject;


class Script_Aranobot : public Script_Enemy
{
public:
	Script_Aranobot();
	Script_Aranobot(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Aranobot();

public:
	virtual bool Start() override;
};
