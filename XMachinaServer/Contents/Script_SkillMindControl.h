#pragma once
#include "Script_Skill.h"

class GameSkill;
class GameObject;

class Script_SkillMindControl : public Script_Skill
{
private:
	sptr<GameMonster>				mMindControlMonster = {};

public:
	Script_SkillMindControl();
	Script_SkillMindControl(SPtr<GameSkill> owner, ScriptInfo::Type type);
	Script_SkillMindControl(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_SkillMindControl();
public:
	virtual void Clone(SPtr<Component> other);

	virtual bool Update();

public:
	void SetMindControlMonster(SPtr<GameMonster> mindControlMonster) { mMindControlMonster = mindControlMonster; }
	SPtr<GameMonster> GetMindControlMonster() { return mMindControlMonster; }
};

