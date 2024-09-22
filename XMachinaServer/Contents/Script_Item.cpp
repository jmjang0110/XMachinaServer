#include "pch.h"
#include "Script_Item.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "Script_Player.h"


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
    if (mItemState != ItemState::Dropped)
        return false;

    const Vec3 playerPos = player->GetTransform()->GetSnapShot().GetPosition();
    const Vec3 ItemPos   = mOwner->GetTransform()->GetPosition();

    int compareDist = 3.f;
    if ((playerPos - ItemPos).Length() <= compareDist) {

        mOwnerPlayer = player;
        auto playerEntity = mOwnerPlayer->GetScriptEntity<Script_Player>();
        playerEntity->AddWeapon(mOwner);
        mItemState   = ItemState::Owned;

        return true;
    }
    return false;
}

bool Script_Item::ThrowAway(SPtr<GameObject> player)
{
    if (!mOwnerPlayer)
        return false;

    if (player->GetID() != mOwnerPlayer->GetID())
        return false;

    Vec3 drop_pos = mOwnerPlayer->GetTransform()->GetSnapShot().GetPosition();
    mOwner->GetTransform()->SetPosition(drop_pos);

    auto playerEntity = mOwnerPlayer->GetScriptEntity<Script_Player>();
    playerEntity->DropWeapon(mOwner->GetID());
    mOwnerPlayer = nullptr;
    mItemState   = ItemState::Dropped;

    return true;
}
