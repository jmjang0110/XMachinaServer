#pragma once
#include "Script_Skill.h"

class GameSkill;
class GameObject;
class Script_SkillCloaking : public Script_Skill
{
private:
	float		mTimer      = 0.f;
	bool		mToggle     = false;
	float		mPheroCost  = 30.f;
public:
	Script_SkillCloaking();
	Script_SkillCloaking(SPtr<GameSkill> owner, ScriptInfo::Type type);
	Script_SkillCloaking(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_SkillCloaking();
public:
	virtual void Clone(SPtr<Component> other);

	virtual bool Update();
	virtual void DeActivate();
public:
	void SetToggle(bool toggle) { mToggle = toggle; }
	bool GetToggle() { return mToggle; }
};

