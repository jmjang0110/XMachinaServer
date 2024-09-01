#include "pch.h"
#include "Script_WeaponMineLauncher.h"
#include "GameObject.h"
#include "Script_SpiderMine.h"


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

void Script_WeaponMineLauncher::Start()
{
    for (int i = 0; i < WeaponInfo::MaxBulletsNum; ++i) {
        int id = i;
        SPtr<GameObject> bullet = MEMORY->Make_Shared<GameObject>(id);
        bullet->AddComponent<Transform>(Component::Type::Transform);
        bullet->AddComponent<Collider>(Component::Type::Collider);
        auto bullet_entity = bullet->SetScriptEntity<Script_SpiderMine>();
        bullet_entity->SetOwnerWeapon(mOwner);
        bullet_entity->SetExplosionDamage(5.f);

        mBullets[i] = bullet;
        bullet->Start();
    }
}

void Script_WeaponMineLauncher::Update()
{
}
