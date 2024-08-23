#pragma once
#include "Script_Entity.h"

class GameObject;
class Script_Item : public Script_Entity
{
protected:
	FBProtocol::ITEM_TYPE	mItemType = FBProtocol::ITEM_TYPE_NONE;

public:
	Script_Item();
	Script_Item(SPtr<GameObject> owner);
	virtual ~Script_Item();

	virtual SPtr<Component> Clone(SPtr<Component> target);

	FBProtocol::ITEM_TYPE GetItemType() { return mItemType; }
};

