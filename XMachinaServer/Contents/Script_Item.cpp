#include "pch.h"
#include "Script_Item.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"

Script_Item::Script_Item()
{
    mKey = uint32_t_ScriptKey(ScriptKey::Item);
}

Script_Item::Script_Item(SPtr<GameObject> owner)
	: Script_Entity(owner, uint32_t_ScriptKey(ScriptKey::Item))
{
}

Script_Item::~Script_Item()
{
}

SPtr<Component> Script_Item::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Item>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Entity::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Weapon" << std::endl;
        return nullptr;
    }
}

bool Script_Item::DoInteract(SPtr<GameObject> player)
{
    return false;
}
