#include "pch.h"
#include "Script_WeaponPipeLine.h"
#include "GameObject.h"


Script_WeaponPipeLine::Script_WeaponPipeLine(SPtr<GameObject> owner)
	: Script_Weapon(owner)
{
	mItemType = FBProtocol::ITEM_TYPE_WEAPON_PIPELINE;

}

Script_WeaponPipeLine::~Script_WeaponPipeLine()
{
}
SPtr<Component> Script_WeaponPipeLine::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_WeaponPipeLine>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Weapon::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Weapon" << std::endl;
        return nullptr;
    }
}

void Script_WeaponPipeLine::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_WeaponPipeLine>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}