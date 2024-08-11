#include "pch.h"
#include "Script_SkillCloaking.h"
#include "GameSkill.h"
#include "GameObject.h"
#include "GamePlayer.h"


Script_SkillCloaking::Script_SkillCloaking()
{
}

Script_SkillCloaking::Script_SkillCloaking(SPtr<GameSkill> owner, ScriptInfo::Type type)
	: Script_Skill(owner, type)
{
}

Script_SkillCloaking::Script_SkillCloaking(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Skill(owner, type)
{
}

Script_SkillCloaking::~Script_SkillCloaking()
{
}

void Script_SkillCloaking::Clone(SPtr<Component> other)
{
}

bool Script_SkillCloaking::Update()
{
	Script_Skill::Update();

	GameSkill::State skillCurrState = mSkillOwner->GetState();

	if (skillCurrState == GameSkill::State::CoolTime_Start) {
		mSkillOwner->RegisterUpdate(mCoolTime);
		mSkillOwner->SetState(GameSkill::State::CoolTime_End);

		return true;
	}
	else if (skillCurrState == GameSkill::State::CoolTime_End) {
		mTimer = 0.f;
		mSkillOwner->DeActivate();
		mSkillOwner->SetState(GameSkill::State::Possible);
		return true;
	}

	if (skillCurrState == GameSkill::State::Active) {
		float deltatime = mSkillOwner->GetDeltaTime();
		mTimer += deltatime;

		LOG_MGR->Cout("Cloaking On : ", mTimer, '\n');
		bool checkOnSkill = mSkillOwner->GetOwnerPlayer()->S_DecPhero(deltatime * mPheroCost);
		if (checkOnSkill == true) {
			mSkillOwner->RegisterUpdate(0.f);
		}
		else {
			mTimer = 0.f;
			mSkillOwner->DeActivate();
			mSkillOwner->SetState(GameSkill::State::Possible);
		}

	}

	return true;
}

void Script_SkillCloaking::DeActivate()
{
	Script::DeActivate();
	mSkillOwner->SetState(GameSkill::State::Possible);

}
