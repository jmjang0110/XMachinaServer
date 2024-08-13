#pragma once
#include "Script_Enemy.h"

class GameObject;


class Script_AdvancedCombatDroid_5 : public Script_Enemy
{
public:
	Script_AdvancedCombatDroid_5();
	Script_AdvancedCombatDroid_5(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_AdvancedCombatDroid_5();

public:
	virtual bool Start() override;
};

