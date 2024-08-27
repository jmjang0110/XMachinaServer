#include "pch.h"
#include "Script_Weapon.h"
#include "GameObject.h"

Script_Weapon::Script_Weapon(SPtr<GameObject> owner)
	: Script_Item(owner)
{
}

Script_Weapon::~Script_Weapon()
{
    for (int i = 0; i < WeaponInfo::MaxBulletsNum; ++i) {
        mBullets[i] = nullptr;
    }
}


void Script_Weapon::Update()
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

int Script_Weapon::OnShoot(Vec3& center_pos, Vec3& fire_dir)
{
    int possibleIndex = -1;
    if (mPossibleBulletIndex.try_pop(possibleIndex)) {

        if (0 <= possibleIndex && possibleIndex < WeaponInfo::MaxBulletsNum) {

            mBullets[possibleIndex]->GetTransform()->SetPosition(center_pos);
            mBullets[possibleIndex]->GetTransform()->SetRight(Vector3::Right);
            mBullets[possibleIndex]->GetTransform()->SetLook(fire_dir);
            mBullets[possibleIndex]->Activate(); // PQCS - Register Update !
            return possibleIndex;
        }
    }

    return -1;
}

bool Script_Weapon::DoInteract(SPtr<GameObject> player)
{
    // 거리차이로 계산
    Vec3 playerPos = player->GetTransform()->GetSnapShot().GetPosition();
    Vec3 itemPos   = mOwner->GetTransform()->GetPosition();
    float dist     = (playerPos - itemPos).Length();
    
    float interactDist = 3.f;
    if (dist <= interactDist)
        return true;

    return false;
}
