#include "pch.h"
#include "Script_Weapon.h"
#include "GameObject.h"

Script_Weapon::Script_Weapon(SPtr<GameObject> owner)
	: Script_Item(owner)
{
}

Script_Weapon::~Script_Weapon()
{
}

SPtr<Component> Script_Weapon::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Weapon>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Item::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Weapon" << std::endl;
        return nullptr;
    }
}

void Script_Weapon::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Weapon>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}