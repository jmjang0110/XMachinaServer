#pragma once
#undef max
#include "Enum_generated.h"

#include "GameObject.h"
#include "ObjectSnapShot.h"


namespace SkillInfo
{
	enum class Type : UINT8 {
		None,

		Cloaking,
		MindControl,
		IRDetector,		/* 적외선 */
		Shield,

		End,
	};

	enum class State : UINT8 {
		Impossible,	// 사용 불가
		Possible,	// 사용 가능 
		Active,		// 사용 중 
	};

}

class GameBullet;
class GamePlayer;
struct  SkillSnapShot : public ObjectSnapShot
{
	GamePlayer*				PlayerOwner = nullptr;
	bool					IsActive    = false; Lock::SRWLock Lock_IsActive;
};

class GameSkill : public GameObject
{
private:
	SPtr<GamePlayer>				mOwnerPlayer = nullptr;
	std::atomic_int					mActivate_Ref	= 0;

private:
	SkillSnapShot					mSnapShot;
	float							mCoolTime       = 0.f;
	float							mActiveDuration = 0.f;
	FBProtocol::PLAYER_SKILL_TYPE	mSkillType      = FBProtocol::PLAYER_SKILL_TYPE_END; 

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
	void DecreaseRef() { mActivate_Ref.fetch_sub(1); if (mActivate_Ref.load() < 0) mActivate_Ref = 0; }


	/// +-----------------------------------------------------------
	///		S E T T E R 
	/// -----------------------------------------------------------+
	void SetCoolTime(float coolTime)							{ mCoolTime       = coolTime; }
	void SetDurationTime(float durationTime)					{ mActiveDuration = durationTime; }
	void SetSkillType(FBProtocol::PLAYER_SKILL_TYPE skilltype)	{ mSkillType      = skilltype; }


	/// +-----------------------------------------------------------
	///		G E T T E R 
	/// -----------------------------------------------------------+
	int						GetActivate_RefCnt() { return mActivate_Ref.load(); }
    SPtr<GamePlayer>		GetOwnerPlayer()	 { return mOwnerPlayer;}
    bool					GetIsActive()		 { mSnapShot.Lock_IsActive.LockRead();  bool active = mSnapShot.IsActive;  mSnapShot.Lock_IsActive.UnlockRead();    return active; }


};

