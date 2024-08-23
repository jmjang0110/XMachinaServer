#pragma once
#include "Script_Weapon.h"

class GameObject;
class Script_WeaponBurnout : public Script_Weapon
{
private:

public:
	Script_WeaponBurnout() = default;
	Script_WeaponBurnout(SPtr<GameObject> owner);
	virtual ~Script_WeaponBurnout();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

};

