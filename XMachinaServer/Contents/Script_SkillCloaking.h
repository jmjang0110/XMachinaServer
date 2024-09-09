#pragma once
#include "Script_Skill.h"

class GameSkill;
class GameObject;
class Component;
class Script_SkillCloaking : public Script_Skill
{
private:
	float		mTimer      = 0.f;
	bool		mToggle     = false;
	float		mPheroCost  = 30.f;
public:
	Script_SkillCloaking() = default;
	Script_SkillCloaking(SPtr<GameObject> owner);
	virtual ~Script_SkillCloaking();
public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
	virtual void Update();

	virtual void Activate() override;
	virtual void DeActivate();
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;


public:
	void SetToggle(bool toggle) { mToggle = toggle; }
	bool GetToggle() { return mToggle; }
};

