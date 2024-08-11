#include "pch.h"
#include "Script_SkillShield.h"
#include "GameSkill.h"
#include "GameObject.h"

Script_SkillShield::Script_SkillShield()
{
}

Script_SkillShield::Script_SkillShield(SPtr<GameSkill> owner, ScriptInfo::Type type)
	: Script_Skill(owner, type)
{
}

Script_SkillShield::Script_SkillShield(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Skill(owner, type)
{
}

Script_SkillShield::~Script_SkillShield()
{
}

void Script_SkillShield::Clone(SPtr<Component> other)
{
}

bool Script_SkillShield::Update()
{
	Script_Skill::Update();
	GameSkill::State skillState = mSkillOwner->GetState();

	if (skillState == GameSkill::State::Active) {
		mSkillOwner->SetState(GameSkill::State::CoolTime_Start);
		mSkillOwner->RegisterUpdate(mActiveDuration);
	}
	else if (skillState == GameSkill::State::CoolTime_Start) {
		mSkillOwner->SetState(GameSkill::State::CoolTime_End);
		mSkillOwner->RegisterUpdate(mCoolTime);
	}
	else if (skillState == GameSkill::State::CoolTime_End) {
		mSkillOwner->SetState(GameSkill::State::Possible);
		mSkillOwner->DeActivate();
	}

	return true;
}
