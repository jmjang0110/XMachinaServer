#include "pch.h"
#include "Script_WeaponMineLauncher.h"
#include "GameObject.h"


Script_WeaponMineLauncher::Script_WeaponMineLauncher(SPtr<GameObject> owner)
	: Script_Weapon(owner)
{
	mItemType = FBProtocol::ITEM_TYPE_WEAPON_MINE_LAUNCHER;

}

Script_WeaponMineLauncher::~Script_WeaponMineLauncher()
{
}
SPtr<Component> Script_WeaponMineLauncher::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_WeaponMineLauncher>(target);
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

void Script_WeaponMineLauncher::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_WeaponMineLauncher>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}