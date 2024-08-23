#include "pch.h"
#include "Script_Building.h"
#include "GameObject.h"

Script_Building::Script_Building()
{
    mKey = uint32_t_ScriptKey(ScriptKey::Building);

}

Script_Building::Script_Building(SPtr<GameObject> owner)
    : Script_Entity(owner, uint32_t_ScriptKey(ScriptKey::Building))
{
    owner->EnableTag(ObjectTag::Building);
    
}

Script_Building::~Script_Building()
{
}

SPtr<Component> Script_Building::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Building
    auto clonedScript = std::dynamic_pointer_cast<Script_Building>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Entity::Clone(clonedScript);

        // Copy the specific member variables
        clonedScript->mSectorIdx = this->mSectorIdx;

        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Building" << std::endl;
        return nullptr;
    }
}

void Script_Building::Clone(SPtr<GameObject> target)
{
    auto clonedScript = target->AddScript<Script_Building>();
    this->Clone(clonedScript);   
    clonedScript->SetOwner(target);

}