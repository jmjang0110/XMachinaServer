#include "pch.h"
#include "GameObject.h"
#include "Script_Phero.h"

Script_Phero::Script_Phero()
{
}

Script_Phero::Script_Phero(SPtr<GameObject> owner)
    : Script_Entity(owner, uint32_t_ScriptKey(ScriptKey::Phero))
{
    owner->EnableTag(ObjectTag::Phero);

}

Script_Phero::~Script_Phero()
{
    mKey = uint32_t_ScriptKey(ScriptKey::Phero);
}


SPtr<Component> Script_Phero::Clone(SPtr<Component> target)
{
    // First, clone the base Script_Entity part
    Script_Entity::Clone(target);

    // Cast the target to the appropriate type (Script_Phero)
    auto pheroScript = std::dynamic_pointer_cast<Script_Phero>(target);

    // Ensure the casting was successful
    if (pheroScript)
    {
        // Copy the Script_Phero-specific member variables
        pheroScript->mLevel          = this->mLevel;
        pheroScript->mLifeTime       = this->mLifeTime;
        pheroScript->mAmount         = this->mAmount;
        pheroScript->mOffsetDist     = this->mOffsetDist;
        pheroScript->mTargetPlayerID = this->mTargetPlayerID.load();  // Atomic variable copied safely
    }

    return target;
}

void Script_Phero::Clone(SPtr<GameObject> target)
{
    // Add a new Script_PheroDropper instance to the GameObject
    auto clonedScript = target->AddScript<Script_Phero>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}
void Script_Phero::Activate()
{

}

void Script_Phero::DeActivate()
{

}

void Script_Phero::Start()
{
    mTargetPlayerID.store(-1);

}

void Script_Phero::Update()
{
}

void Script_Phero::LateUpdate()
{
}

void Script_Phero::End()
{
}

void Script_Phero::Init(int monster_id, int phero_index, int level, float lifeTime, float amount)
{
    int phero_id = monster_id * PheroInfo::MaxPheroCreateCount + phero_index;
    GetOwner()->SetID(phero_id);

    mLevel    = level;
    mLifeTime = lifeTime;
    mAmount   = amount;

}
