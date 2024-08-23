#include "pch.h"
#include "Script_Entity.h"
#include "GameObject.h"

Script_Entity::Script_Entity(SPtr<GameObject> owner, UINT32 key)
    : Script(owner, key)
{
}

Script_Entity::~Script_Entity()
{
}

void Script_Entity::Activate()
{
	mActivateRef.fetch_add(1);

}

void Script_Entity::DeActivate()
{
	mActivateRef.fetch_sub(1);

}
SPtr<Component> Script_Entity::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Entity
    auto clonedScript = std::dynamic_pointer_cast<Script_Entity>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script::Clone(clonedScript);

        // Copy the specific member variables
        clonedScript->mTag         = this->mTag;
        clonedScript->mSectorIndex = this->mSectorIndex;
        clonedScript->mActivateRef = this->mActivateRef.load();

        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Entity" << std::endl;
        return nullptr;
    }
}
