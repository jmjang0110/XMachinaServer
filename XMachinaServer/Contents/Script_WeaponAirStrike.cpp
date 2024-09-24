#include "pch.h"
#include "Script_WeaponAirStrike.h"
#include "Script_BasicBullet.h"
#include "GameObject.h"
#include "Script_BulletAirStrike.h"


Script_WeaponAirStrike::Script_WeaponAirStrike(SPtr<GameObject> owner)
    : Script_Weapon(owner)
{

#ifdef SET_DATA_FROM_DATABASE
    Script_Weapon::SetDataFromDataBase(L"AirStrike");
#else

#endif

}

Script_WeaponAirStrike::~Script_WeaponAirStrike()
{
}

SPtr<Component> Script_WeaponAirStrike::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_WeaponAirStrike>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Weapon::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_WeaponAirStrike" << std::endl;
        return nullptr;
    }
}

void Script_WeaponAirStrike::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_WeaponAirStrike>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}


void Script_WeaponAirStrike::Start()
{
    for (int i = 0; i < WeaponInfo::MaxBulletsNum; ++i) {
        int id = i;
        SPtr<GameObject> bullet = MEMORY->Make_Shared<GameObject>(id);
        bullet->AddComponent<Transform>(Component::Type::Transform);
        bullet->AddComponent<Collider>(Component::Type::Collider);
        bullet->SetOwnerRoom(mOwner->GetOwnerRoom());
        auto bullet_entity = bullet->SetScriptEntity<Script_BulletAirStrike>();
        bullet_entity->SetDamage(mAttackRate);
        bullet_entity->SetOwnerWeapon(mOwner);
        mBullets[i] = bullet;
        bullet->Start();

        mPossibleBulletIndex.push(i);
    }
}

void Script_WeaponAirStrike::Update()
{
}
