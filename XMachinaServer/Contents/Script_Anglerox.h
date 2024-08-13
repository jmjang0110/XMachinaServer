#pragma once

#include "Script_Enemy.h"

class GameObject;

class Script_Anglerox : public Script_Enemy
{
public:
	Script_Anglerox();
	Script_Anglerox(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Anglerox();

public:
	virtual bool Start() override;
};


