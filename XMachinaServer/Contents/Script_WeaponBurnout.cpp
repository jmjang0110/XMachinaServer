#include "pch.h"
#include "Script_WeaponBurnout.h"
#include "GameObject.h"


Script_WeaponBurnout::Script_WeaponBurnout(SPtr<GameObject> owner)
	: Script_Weapon(owner)
{
	mItemType = FBProtocol::ITEM_TYPE_WEAPON_BURNOUT;
}

Script_WeaponBurnout::~Script_WeaponBurnout()
{
}

SPtr<Component> Script_WeaponBurnout::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_WeaponBurnout>(target);
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

void Script_WeaponBurnout::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_WeaponBurnout>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}