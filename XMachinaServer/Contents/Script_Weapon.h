#pragma once
#include "Script_Item.h"


class GameObject;
class Script_Weapon : public Script_Item
{
private:

public:
	Script_Weapon() = default;
	Script_Weapon(SPtr<GameObject> owner);
	virtual ~Script_Weapon();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

};

