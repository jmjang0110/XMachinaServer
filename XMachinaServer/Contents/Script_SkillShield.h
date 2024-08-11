#pragma once
#include "Script_Skill.h"

class GameSkill;
class GameObject;
class Script_SkillShield : public Script_Skill
{
private:

public:
	Script_SkillShield();
	Script_SkillShield(SPtr<GameSkill> owner, ScriptInfo::Type type);
	Script_SkillShield(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_SkillShield();

public:
	virtual void Clone(SPtr<Component> other);

	virtual bool Update();
};

