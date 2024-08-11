#include "pch.h"
#include "Script_Skill.h"
#include "GameSkill.h"
#include "GameObject.h"

Script_Skill::Script_Skill()
{
}

Script_Skill::Script_Skill(SPtr<GameSkill> owner, ScriptInfo::Type type)
	: Script(owner, type, static_cast<UINT32>(type))
{
	mSkillOwner = owner.get();
}

Script_Skill::Script_Skill(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script(owner, type, static_cast<UINT32>(type))
{
}
Script_Skill::~Script_Skill()
{
}

void Script_Skill::Clone(SPtr<Component> other)
{
}



bool Script_Skill::Update()
{
	Script::Update();

	return true;
}

