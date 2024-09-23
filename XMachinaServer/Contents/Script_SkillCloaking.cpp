#include "pch.h"
#include "Script_SkillCloaking.h"
#include "Script_Player.h"
#include "Script_PlayerStat.h"
#include "GameObject.h"

Script_SkillCloaking::Script_SkillCloaking(SPtr<GameObject> owner)
	: Script_Skill(owner)
{
	mSkillType = FBProtocol::PLAYER_SKILL_TYPE_CLOACKING;

#ifdef SET_DATA_FROM_DATABASE
	Script_Skill::SetDataFromDataBase("Cloaking");
	mCoolTime -= 1.f;
	mActiveDuration -= 1.f;
#else

#endif
}

Script_SkillCloaking::~Script_SkillCloaking()
{
}

SPtr<Component> Script_SkillCloaking::Clone(SPtr<Component> target)
{
	// Clone the base class part (Script_Skill)
	Script_Skill::Clone(target);

	// Cast the target to the appropriate type (Script_SkillCloaking)
	auto skillCloaking = std::dynamic_pointer_cast<Script_SkillCloaking>(target);

	// Ensure the casting was successful
	if (skillCloaking)
	{
		// Copy the Script_SkillCloaking-specific member variables
		skillCloaking->mTimer     = this->mTimer;
		skillCloaking->mToggle    = this->mToggle;
		skillCloaking->mPheroCost = this->mPheroCost;
	}

	return target;
}

void Script_SkillCloaking::Clone(SPtr<GameObject> target)
{
	// Add a new Script_PlayerStat instance to the GameObject
	auto clonedScript = target->AddScript<Script_SkillCloaking>();
	// Clone the current script into the new script
	this->Clone(clonedScript);
}

void Script_SkillCloaking::Update()
{
	Script_Skill::Update();

	switch (mSkillState)
	{
	case SkillState::Impossible:
	case SkillState::Possible: {
		auto playerPhero = mOwnerPlayer->GetScriptEntity<Script_Player>()->GetPhero();
		if (playerPhero >= 0.f) {
			mSkillState = SkillState::Active;
			mOwner->RegisterUpdate(0.f);
		}
	}
		break;
	case SkillState::Active:
	{
		bool checkOnSkill = mOwnerPlayer->GetScriptEntity<Script_Player>()->ReducePheroAmount(DeltaTime() * mPheroCost);
		if (checkOnSkill == true) {
			mOwner->RegisterUpdate(0.f);
		} 
		else {
			mSkillState = SkillState::Possible;
			mOwner->DeActivate();
		}
	}
		break;
	case SkillState::CoolTime_Start:
	{
		mSkillState = SkillState::CoolTime_End;
		mOwner->RegisterUpdate(mCoolTime);
	}
		break;
	case SkillState::CoolTime_End:
	{
		mTimer = 0.f;
		mSkillState = SkillState::Possible;
		mOwner->DeActivate();
	}
		break;
	default:
		assert(0);
		break;
	}
}


void Script_SkillCloaking::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
	MEMORY->Delete(overlapped);

	mOwner->Update();
}

void Script_SkillCloaking::Activate()
{
	Script::Activate();

	mSkillState = SkillState::Possible;
}

void Script_SkillCloaking::DeActivate()
{
	Script::DeActivate();

	mTimer = 0.f;
	mSkillState = SkillState::Possible;

}
