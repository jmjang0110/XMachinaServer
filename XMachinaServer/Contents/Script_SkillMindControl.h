#pragma once
#include "Script_Skill.h"

class GameSkill;
class GameObject;
class Script_SkillMindControl : public Script_Skill
{
private:
	SPtr<GameObject>				mMindControlMonster = {};

public:
	Script_SkillMindControl() = default;
	Script_SkillMindControl(SPtr<GameObject> owner);
	virtual ~Script_SkillMindControl();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Update();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	void Init(SPtr<GameObject> monster);


public:
	void SetMindControlMonster(SPtr<GameObject> mindControlMonster) { mMindControlMonster = mindControlMonster; }
	SPtr<GameObject> GetMindControlMonster() { return mMindControlMonster; }
};

