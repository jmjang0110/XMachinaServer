#include "pch.h"
#include "Script_Crate.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "NPCController.h"
#include "ResourceManager.h"
#include "FBsPacketFactory.h"
#include "RoomManager.h"
#include "GameRoom.h"

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
        if (this->mItem)
            clonedScript->mItemID = this->mItem->GetID();
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

void Script_Crate::Start()
{
    CloseCrate();
    mItem = mOwner->GetOwnerRoom()->GetNPCController()->GetDynamicItem(mItemID);
}

bool Script_Crate::DoInteract(SPtr<GameObject> player)
{
    Vec3 player_position = player->GetTransform()->GetSnapShot().GetPosition();
    Vec3 crate_position  = mOwner->GetTransform()->GetPosition();
    Vec3 dist            = player_position - crate_position;
    // dist의 길이를 계산하고 3.f 이내면 true 반환
    if (dist.Length() <= 3.f) {
        if (mCrateState == CrateState::Closed) {
            mCrateState = CrateState::Open;
            if (DropItem(player)) {

            }
        }
        else if (mCrateState == CrateState::Open) {
            mCrateState = CrateState::Closed;
        }
    }
    return true;
}

bool Script_Crate::DropItem(SPtr<GameObject> player)
{
    auto npcController = mOwner->GetOwnerRoom()->GetNPCController();

    // LOCK ! 

    /* There is no item in crate */
    mLock_Item.LockWrite();

    if (!mItem) {
        mLock_Item.UnlockWrite();
        return false;
    }

    npcController->AddDroppedITem(mItem->GetID(), mItem);

    // 상자가 열림 
    auto spkt = FBS_FACTORY->SPkt_Item_Interact(player->GetID(), mOwner->GetID(), mItemType, mOwner->GetTransform()->GetPosition());
    ROOM_MGR->BroadcastRoom(player->GetOwnerRoom()->GetID(), spkt);

    // 아이템이 드랍됐음을 알린다.
    auto item_entity = mItem->GetScriptEntity<Script_Item>();
    spkt = FBS_FACTORY->SPkt_Item_ThrowAway(0, mItem->GetID(), item_entity->GetItemType(), mItem->GetTransform()->GetPosition());
    ROOM_MGR->BroadcastRoom(player->GetOwnerRoom()->GetID(), spkt);

    mItem   = nullptr;
    mItemID = -1;

    mLock_Item.UnlockWrite();
    return true;
}
