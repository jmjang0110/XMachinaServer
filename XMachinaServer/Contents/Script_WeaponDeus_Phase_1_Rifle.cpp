#include "pch.h"
#include "Script_WeaponDeus_Phase_1_Rifle.h"
#include "Script_BulletDeus_Phase_1.h"
#include "GameObject.h"


Script_WeaponDeus_Phase_1_Rifle::Script_WeaponDeus_Phase_1_Rifle(SPtr<GameObject> owner)
    : Script_Weapon(owner)
{
    Script_Weapon::mAttackRate = 20.f;

}

Script_WeaponDeus_Phase_1_Rifle::~Script_WeaponDeus_Phase_1_Rifle()
{
}

SPtr<Component> Script_WeaponDeus_Phase_1_Rifle::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_WeaponDeus_Phase_1_Rifle>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Weapon::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_WeaponDeus_Phase_1_Rifle" << std::endl;
        return nullptr;
    }
}

void Script_WeaponDeus_Phase_1_Rifle::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_WeaponDeus_Phase_1_Rifle>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}

void Script_WeaponDeus_Phase_1_Rifle::Start()
{
    for (int i = 0; i < WeaponInfo::MaxBulletsNum; ++i) {
        int id = i;
        SPtr<GameObject> bullet = MEMORY->Make_Shared<GameObject>(id);
        bullet->AddComponent<Transform>(Component::Type::Transform);
        bullet->AddComponent<Collider>(Component::Type::Collider);
        bullet->SetOwnerRoom(mOwner->GetOwnerRoom());
        auto bullet_entity = bullet->SetScriptEntity<Script_BulletDeus_Phase_1>();
        bullet_entity->SetOwnerWeapon(mOwner);
        mBullets[i] = bullet;
        bullet->Start();

        mPossibleBulletIndex.push(i);
    }
}

void Script_WeaponDeus_Phase_1_Rifle::Update()
{
}
