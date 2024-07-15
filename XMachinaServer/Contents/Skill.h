#pragma once
#include "GameObject.h"

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

class Skill 
{
private:
	SPtr<GameObject> mOwner			= nullptr;		// 스킬 소유 게임 오브젝트 ( 플레이어 )

private:
	SkillInfo::Type	 mType          = SkillInfo::Type::None;			// 스킬 타입 
	SkillInfo::State mState         = SkillInfo::State::Possible;		// 사용 가능, 사용 중, 사용 불가능  

	float			mCoolTime       = 0.f;							    // 쿨 타임 
	float			mActiveDuration = 0.f;							    // 스킬 지속 시간 

public:
	Skill();
	Skill(SPtr<GameObject> owner, SkillInfo::Type type, float cooltime, float activeduration);
	~Skill();

public:
	void Update();

	SkillInfo::State GetState() { return mState; }

};

