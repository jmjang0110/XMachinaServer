#pragma once
#undef max
#include "Enum_generated.h"
#include "Script.h"

class GameSkill;
class GameObject;
class Script_Skill : public Script
{
protected:
	GameSkill*						mSkillOwner     = nullptr;
	FBProtocol::PLAYER_SKILL_TYPE	mSkillType      = FBProtocol::PLAYER_SKILL_TYPE_END;
	float							mCoolTime       = 0.f;
	float							mActiveDuration = 0.f;
	float							mTimer          = 0.f;

public:
	Script_Skill();
	Script_Skill(SPtr<GameSkill> owner, ScriptInfo::Type type);
	Script_Skill(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_Skill();

public:
	virtual void Clone(SPtr<Component> other);

	virtual bool Update();

public:
	void SetSkillOwner(GameSkill* skillOwner) { mSkillOwner = skillOwner; }
	void SetSkillType(FBProtocol::PLAYER_SKILL_TYPE skillType) { mSkillType = skillType; }
	void SetCoolTime(float coolTime) { mCoolTime = coolTime; }
	void SetDurationTime(float durationTime) { mActiveDuration = durationTime; }


	FBProtocol::PLAYER_SKILL_TYPE GetSkillType() { return mSkillType; }
	float GetCoolTime() { return mCoolTime; }
	float GetActiveDuration() { return mActiveDuration; }
};

