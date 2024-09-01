#include "pch.h"
#include "Script_WeaponHLock.h"
#include "Script_BasicBullet.h"
#include "GameObject.h"

Script_WeaponHLock::Script_WeaponHLock(SPtr<GameObject> owner)
{
    mItemType = FBProtocol::ITEM_TYPE_WEAPON_H_LOOK;

}

Script_WeaponHLock::~Script_WeaponHLock()
{
}

void Script_WeaponHLock::Start()
{
    for (int i = 0; i < WeaponInfo::MaxBulletsNum; ++i) {
        int id = i;
        SPtr<GameObject> bullet = MEMORY->Make_Shared<GameObject>(id);
        bullet->AddComponent<Transform>(Component::Type::Transform);
        bullet->AddComponent<Collider>(Component::Type::Collider);
        auto bullet_entity = bullet->SetScriptEntity<Script_BasicBullet>();
        bullet_entity->SetOwnerWeapon(mOwner);
        mBullets[i] = bullet;
        bullet->Start();
    }
}

void Script_WeaponHLock::Update()
{
}

SPtr<Component> Script_WeaponHLock::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_WeaponHLock>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Weapon::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_WeaponHLock" << std::endl;
        return nullptr;
    }
}

void Script_WeaponHLock::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_WeaponHLock>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}

