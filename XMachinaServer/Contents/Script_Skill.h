#pragma once
#include "Script_Entity.h"

enum class SkillState : UINT8 {
	None,
	/* 사용 불가 */
	Impossible,
	/* 사용 가능*/
	Possible,
	/* 사용 중*/
	Active,
	/* 쿨 타임*/
	CoolTime_Start,
	CoolTime_End,
	_count,
};

class GameObject;
class Script_Skill : public Script_Entity
{
protected:
	SPtr<GameObject>				mOwnerPlayer	= nullptr; // Skill 을 소유한 Player 
	FBProtocol::PLAYER_SKILL_TYPE	mSkillType      = FBProtocol::PLAYER_SKILL_TYPE_NONE;
	SkillState						mSkillState     = SkillState::None;

	float							mCoolTime         = {};
	float							mActiveDuration   = {};
	float							mTimer            = {};

	float							mPheroAmountToUse = {};
public:
	Script_Skill();
	Script_Skill(SPtr<GameObject> owner);
	virtual ~Script_Skill();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);
	virtual void Update();

public:
	void SetSkillType(FBProtocol::PLAYER_SKILL_TYPE skillType)	{ mSkillType      = skillType; }
	void SetCoolTime(float coolTime)							{ mCoolTime       = coolTime; }
	void SetDurationTime(float durationTime)					{ mActiveDuration = durationTime; }
	void SetOwnerPlayer(SPtr<GameObject> owner_player)			{ mOwnerPlayer    = owner_player; }

	FBProtocol::PLAYER_SKILL_TYPE	GetSkillType()				{ return mSkillType; }
	SkillState						GetCurrSkillState()			{ return mSkillState; }
	float							GetCoolTime()				{ return mCoolTime; }
	float							GetActiveDuration()			{ return mActiveDuration; }
	SPtr<GameObject>				GetOwnerPlayer()			{ return mOwnerPlayer; }

	void SetDataFromDataBase(std::string skill_name);
};

