#include "pch.h"
#include "Script_Skill.h"
#include "GameObject.h"
#include "Script_Stat.h"
#include "Script_Player.h"
#include "DBController.h"
#include "ResourceManager.h"
#include "DB_Skill.h"

Script_Skill::Script_Skill()
{
    mKey = uint32_t_ScriptKey(ScriptKey::Skill);

}

Script_Skill::Script_Skill(SPtr<GameObject> owner)
	: Script_Entity(owner, uint32_t_ScriptKey(ScriptKey::Skill))
{
	owner->EnableTag(ObjectTag::Skill);
}
Script_Skill::~Script_Skill()
{
}

SPtr<Component> Script_Skill::Clone(SPtr<Component> target)
{
    // First, clone the base Script_Entity part
    Script_Entity::Clone(target);

    // Cast the target to the appropriate type (Script_Skill)
    auto skillScript = std::dynamic_pointer_cast<Script_Skill>(target);

    // Ensure the casting was successful
    if (skillScript)
    {
        // Copy the Script_Skill-specific member variables
        skillScript->mSkillType      = this->mSkillType;
        skillScript->mCoolTime       = this->mCoolTime;
        skillScript->mActiveDuration = this->mActiveDuration;
        skillScript->mTimer          = this->mTimer;
    }

    return target;
}

void Script_Skill::Clone(SPtr<GameObject> target)
{
    // Add a new Script_PlayerStat instance to the GameObject
    auto clonedScript = target->AddScript<Script_Skill>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}

void Script_Skill::Update()
{
	float deltatime = mOwner->DeltaTime();
	mTimer += deltatime;

}

void Script_Skill::SetDataFromDataBase(std::string skill_name)
{

    auto DB = RESOURCE_MGR->GetSkillInfo(skill_name);
    mOwner->SetName(skill_name);
    mActiveDuration = DB->ActiveDurationTime;
    mCoolTime       = DB->CoolTime;

}

