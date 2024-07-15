#pragma once
#include "GameObject.h"

namespace SkillInfo
{
	enum class Type : UINT8 {
		None, 

		Cloaking,
		MindControl,
		IRDetector,		/* ���ܼ� */
		Shield,
		
		End,
	};

	enum class State : UINT8 {
		Impossible,	// ��� �Ұ�
		Possible,	// ��� ���� 
		Active,		// ��� �� 
	};

}

class Skill 
{
private:
	SPtr<GameObject> mOwner			= nullptr;		// ��ų ���� ���� ������Ʈ ( �÷��̾� )

private:
	SkillInfo::Type	 mType          = SkillInfo::Type::None;			// ��ų Ÿ�� 
	SkillInfo::State mState         = SkillInfo::State::Possible;		// ��� ����, ��� ��, ��� �Ұ���  

	float			mCoolTime       = 0.f;							    // �� Ÿ�� 
	float			mActiveDuration = 0.f;							    // ��ų ���� �ð� 

public:
	Skill();
	Skill(SPtr<GameObject> owner, SkillInfo::Type type, float cooltime, float activeduration);
	~Skill();

public:
	void Update();

	SkillInfo::State GetState() { return mState; }

};

