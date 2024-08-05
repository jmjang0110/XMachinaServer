#pragma once
#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "ObjectSnapShot.h"


class GameBullet;
class GamePlayer;
class GameSkill : public GameObject
{
public:
	enum class State : UINT8 {
		/* 사용 불가 */ 
		Impossible,	    
		/* 사용 가능*/
		Possible ,
		/* 사용 중*/
		Active, };
private:
	SPtr<GamePlayer>				mOwnerPlayer = nullptr;
	std::atomic_int					mActivate_Ref	= 0;

private:
	float							mConsumePheroAmount = 10.f; // test
	float							mCoolTime           = 0.f;
	float							mActiveDuration     = 0.f;
	FBProtocol::PLAYER_SKILL_TYPE	mSkillType          = FBProtocol::PLAYER_SKILL_TYPE_END; 
	GameSkill::State				mSkillState         = GameSkill::State::Possible; Lock::SRWLock Lock_SkillState;

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
	bool OnSkill(float playerTotalPhero);


public:
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }


	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetCoolTime(float coolTime)							{ mCoolTime       = coolTime; }
	void SetDurationTime(float durationTime)					{ mActiveDuration = durationTime; }
	void SetSkillType(FBProtocol::PLAYER_SKILL_TYPE skilltype)	{ mSkillType      = skilltype; }
	void SetSNS_State(GameSkill::State state)						{ Lock_SkillState.LockWrite(); mSkillState = state; Lock_SkillState.UnlockWrite(); }

	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	int						GetActivate_RefCnt() { return mActivate_Ref.load(); }
    SPtr<GamePlayer>		GetOwnerPlayer()	 { return mOwnerPlayer;}
	GameSkill::State		GetSNS_State()		 { Lock_SkillState.LockRead(); GameSkill::State state = mSkillState; Lock_SkillState.UnlockRead(); return state; }

};

