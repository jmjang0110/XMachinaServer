#include "pch.h"
#include "Script_Crate.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "NPCController.h"


Script_Crate::Script_Crate(SPtr<GameObject> owner)
	: Script_Item(owner)
{
	mItemType = FBProtocol::ITEM_TYPE_STATIC_ITEM_CRATE;
}

Script_Crate::~Script_Crate()
{
	mItem = nullptr;
}

SPtr<Component> Script_Crate::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Crate>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Item::Clone(clonedScript);
        clonedScript->mCrateState = this->mCrateState;
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Crate" << std::endl;
        return nullptr;
    }
}

void Script_Crate::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Crate>();
    // Clone the current script into the new script
    this->Clone(clonedScript);
}

bool Script_Crate::DoInteract(SPtr<GameObject> player)
{
    Vec3 player_position = player->GetTransform()->GetSnapShot().GetPosition();
    Vec3 crate_position  = mOwner->GetTransform()->GetPosition();
    Vec3 dist            = player_position - crate_position;

    // dist의 길이를 계산하고 3.f 이내면 true 반환
    float distance  = dist.Length();
    bool interact   = distance <= 3.f;
    if (interact) {
        if (mCrateState == CrateState::Closed) {
            mCrateState = CrateState::Open;
            if(mItem)
                DropItem();
        }
        else if (mCrateState == CrateState::Open) {
            mCrateState = CrateState::Closed;
        }
    }
    return interact;
}

void Script_Crate::DropItem()
{
    auto npcController = mOwner->GetOwnerRoom()->GetNPCController();

    // LOCK ! 

    /* There is no item in crate */
    mLock_Item.LockWrite();

    if (!mItem)
        return;

    npcController->AddDroppedITem(mItem->GetID(), mItem);
    mItem = nullptr;

    mLock_Item.UnlockWrite();

}
