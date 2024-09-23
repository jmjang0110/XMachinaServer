#pragma once
#include "Script_Skill.h"

class GameSkill;
class GameObject;
class Script_SkillShield : public Script_Skill
{
private:

public:
	Script_SkillShield() = default;
	Script_SkillShield(SPtr<GameObject> owner);
	virtual ~Script_SkillShield();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void End();

	virtual void Activate() override;
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
};

