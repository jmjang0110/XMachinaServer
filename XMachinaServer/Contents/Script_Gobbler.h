#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Gobbler : public Script_Enemy
{
public:
	Script_Gobbler();
	Script_Gobbler(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Gobbler();

public:
	virtual bool Start() override;
};
