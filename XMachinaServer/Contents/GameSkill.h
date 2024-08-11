#pragma once
#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "ObjectSnapShot.h"


class GameBullet;
class GamePlayer;
class GameMonster;
class Script_Skill;
class GameSkill : public GameObject
{
public:
	enum class State : UINT8 {
		/* 사용 불가 */ 
		Impossible,	    
		/* 사용 가능*/
		Possible ,
		/* 사용 중*/
		Active, 
		/* 쿨 타임*/
		CoolTime_Start,
		CoolTime_End,
		_count,
	};
private:
	SPtr<GamePlayer>				mOwnerPlayer = nullptr;
	std::atomic_int					mActivate_Ref	= 0;

private:
	float							mConsumePheroAmount = 10.f; // test

	FBProtocol::PLAYER_SKILL_TYPE	mSkillType          = FBProtocol::PLAYER_SKILL_TYPE_END; 
	GameSkill::State				mSkillState         = GameSkill::State::Possible; Lock::SRWLock Lock_SkillState;

	Script_Skill*					mSkillScript = nullptr;
public:
	GameSkill();
	GameSkill(UINT32 sessionID);
	GameSkill(UINT32 sessionID, SPtr<GamePlayer> owner);
	~GameSkill();

public:
	virtual void Update() override;

	virtual void Activate();
	virtual void DeActivate();

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

public:
	bool OnSkill(float playerTotalPhero, SPtr<GameMonster> monster = nullptr);

public:
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }
	void RegisterUpdate(float duration);


	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+

	void SetSkillType(FBProtocol::PLAYER_SKILL_TYPE skilltype)			{ mSkillType                               = skilltype; }
	void SetState(GameSkill::State state)								{ mSkillState = state; }
	void SetSkillScript(Script_Skill* script)							{ mSkillScript = script; }
	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	int								GetActivate_RefCnt() { return mActivate_Ref.load(); }
    SPtr<GamePlayer>				GetOwnerPlayer()	 { return mOwnerPlayer;}
	GameSkill::State				GetState()			 { GameSkill::State state = mSkillState;  return state; }
	FBProtocol::PLAYER_SKILL_TYPE	GetSkillType()		 { return mSkillType; }

	void SetCoolTime(float time);
	void SetActiveDurationTime(float time);

};

