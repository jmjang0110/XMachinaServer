#include "pch.h"
#include "Script_Weapon.h"
#include "GameObject.h"
#include "Transform.h"
#include "FBsPacketFactory.h"
#include "RoomManager.h"
#include "GameRoom.h"


#include "Script_RayCheckBullet.h"
#include "Script_Player.h"
#include "ResourceManager.h"
#include "DB_Weapon.h"

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


void Script_Weapon::Start()
{

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
        clonedScript->mAttackRate         = this->mAttackRate;
        clonedScript->mMaxReloadTime      = this->mMaxReloadTime;
        clonedScript->mMaxDistance        = this->mMaxDistance;
        clonedScript->mMaxMag             = this->mMaxMag;
        clonedScript->mBulletCountPerMag  = this->mBulletCountPerMag;
        clonedScript->mBulletCountPerShot = this->mBulletCountPerShot;

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

int Script_Weapon::OnHitEnemy(int32_t checktargetID, Vec3& center_pos, Vec3& fire_dir)
{
    // Ray check 

    int possibleIndex = -1;
    if (mPossibleBulletIndex.try_pop(possibleIndex)) {

        if (0 <= possibleIndex && possibleIndex < WeaponInfo::MaxBulletsNum) {

            if (mBullets[possibleIndex]->IsActive() == true)
                return -1;

            mBullets[possibleIndex]->GetTransform()->SetPosition(center_pos);
            mBullets[possibleIndex]->GetTransform()->SetRight(Vector3::Right);
            mBullets[possibleIndex]->GetTransform()->SetLook(fire_dir);
            auto raycheck = mBullets[possibleIndex]->GetScriptEntity<Script_RayCheckBullet>();
            if (raycheck) {
                raycheck->SetRayCheckTargetID(checktargetID);
                mBullets[possibleIndex]->Activate(); // PQCS - Register Update !
                return possibleIndex;
            }
        }
    }

    return -1;
}

int Script_Weapon::OnShoot(Vec3& center_pos, Vec3& fire_dir)
{
 
	int possibleIndex = -1;
	if (mPossibleBulletIndex.try_pop(possibleIndex)) {

		if (0 <= possibleIndex && possibleIndex < WeaponInfo::MaxBulletsNum) {

			if (mBullets[possibleIndex]->IsActive() == true)
				return -1;

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
    if (dist <= interactDist) {
        auto player_entity = player->GetScriptEntity<Script_Player>();
        player_entity->SetWeapon(mOwner);
        mOwnerPlayer = player;

        // [Broadcast Packet] Item Interact (Update Current Equiped Weapon)
        auto spkt = FBS_FACTORY->SPkt_Item_Interact(player->GetID(), mOwner->GetID(), mItemType, itemPos);
        ROOM_MGR->BroadcastRoom(player->GetOwnerRoom()->GetID(), spkt);
        return true;
    }

    return false;
}

void Script_Weapon::SetDataFromDataBase(std::wstring weapon_name)
{
	auto weapon_db            = RESOURCE_MGR->GetWeaponInfo(weapon_name);
    this->mAttackRate         = weapon_db->AttackRate;
    this->mMaxReloadTime      = weapon_db->MaxReloadTime;
    this->mMaxDistance        = weapon_db->MaxDistance;
    this->mMaxMag             = weapon_db->MaxMag;
    this->mBulletCountPerMag  = weapon_db->BulletCountPerMag;
    this->mBulletCountPerShot = weapon_db->BulletCountPerShot;
}

void Script_Weapon::ReturnPossibleBulletIndex(int index)
{
    mPossibleBulletIndex.push(index);
}

