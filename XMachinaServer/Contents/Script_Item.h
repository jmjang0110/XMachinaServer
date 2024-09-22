#pragma once
#include "Script_Entity.h"

class GameObject;
enum class ItemState {
	None, 
	Dropped, 
	Owned,
	Using,
	
	_count,
};
class Script_Item : public Script_Entity
{
protected:
	FBProtocol::ITEM_TYPE	mItemType    = FBProtocol::ITEM_TYPE_NONE;
	ItemState				mItemState   = ItemState::None;
	SPtr<GameObject>		mOwnerPlayer = nullptr;

public:
	Script_Item();
	Script_Item(SPtr<GameObject> owner);
	virtual ~Script_Item();

	virtual SPtr<Component> Clone(SPtr<Component> target);

	FBProtocol::ITEM_TYPE GetItemType() { return mItemType; }
	void SetItemState(ItemState state) { mItemState = state; }
public:
	virtual bool DoInteract(SPtr<GameObject> player);
	virtual bool ThrowAway(SPtr<GameObject> player);


};

