#include "pch.h"
#include "Script_SkillMindControl.h"
#include "GameSkill.h"
#include "GameObject.h"
#include "GameMonster.h"
#include "GamePlayer.h"
#include "Script_EnemyController.h"



Script_SkillMindControl::Script_SkillMindControl()
{
}

Script_SkillMindControl::Script_SkillMindControl(SPtr<GameSkill> owner, ScriptInfo::Type type)
	: Script_Skill(owner, type)
{
}

Script_SkillMindControl::Script_SkillMindControl(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Skill(owner, type)
{
}

Script_SkillMindControl::~Script_SkillMindControl()
{
}

void Script_SkillMindControl::Clone(SPtr<Component> other)
{
}

bool Script_SkillMindControl::Update()
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

void Script_SkillMindControl::Init(SPtr<GameMonster> monster)
{
	if (monster == nullptr)
		assert(0);

	auto Invoker = mSkillOwner->GetOwnerPlayer();
	monster->SetMindControlled(true);
	monster->GetEnemyController()->SetInvoker(Invoker);
	SetMindControlMonster(monster);
}
