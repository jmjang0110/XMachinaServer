#include "pch.h"
#include "Script_SkillShield.h"
#include "GameObject.h"

Script_SkillShield::Script_SkillShield(SPtr<GameObject> owner)
	: Script_Skill(owner)
{
	mSkillType = FBProtocol::PLAYER_SKILL_TYPE_SHIELD;
}

Script_SkillShield::~Script_SkillShield()
{
}

void Script_SkillShield::Start()
{
}

SPtr<Component> Script_SkillShield::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_Anglerox
	auto clonedScript = std::dynamic_pointer_cast<Script_SkillShield>(target);
	if (clonedScript)
	{
		// Call the base class Clone method first
		Script_Skill::Clone(clonedScript);
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_SkillShield" << std::endl;
		return nullptr;
	}
}

void Script_SkillShield::Clone(SPtr<GameObject> target)
{
	// Add a new Script_Anglerox instance to the GameObject
	auto clonedScript = target->AddScript<Script_SkillShield>();
	// Clone the current script into the new script
	this->Clone(clonedScript);
}

void Script_SkillShield::Update()
{
	Script_Skill::Update();

	switch (mSkillState)
	{
	case SkillState::Impossible:
	case SkillState::Possible:
		break;
	case SkillState::Active:
	{
		mSkillState = SkillState::CoolTime_Start;
		mOwner->RegisterUpdate(mActiveDuration); // call after activeduration time
	}
		break;
	case SkillState::CoolTime_Start:
	{
		mSkillState = SkillState::CoolTime_End;
		mOwner->RegisterUpdate(mCoolTime);		// call after cool time
	}
		break;
	case SkillState::CoolTime_End:
	{
		mSkillState = SkillState::Possible;
		mOwner->DeActivate();
	}
		break;
	default:
		assert(0);
		break;
	}

}

void Script_SkillShield::LateUpdate()
{
}

void Script_SkillShield::End()
{
}

void Script_SkillShield::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}
