#include "pch.h"
#include "Script_Crate.h"
#include "GameObject.h"

Script_Crate::Script_Crate(SPtr<GameObject> owner)
	: Script_Item(owner)
{
	mItemType = FBProtocol::ITEM_TYPE_STATIC_ITEM_CRATE;
}

Script_Crate::~Script_Crate()
{
	mItem = nullptr;
}

SPtr<Component> Script_Crate::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Crate>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Item::Clone(clonedScript);
        clonedScript->mCrateState = this->mCrateState;
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Crate" << std::endl;
        return nullptr;
    }
}

void Script_Crate::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Crate>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}