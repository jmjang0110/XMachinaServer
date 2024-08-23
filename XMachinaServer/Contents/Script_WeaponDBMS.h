#pragma once
#include "Script_Weapon.h"

class GameObject;
class Script_WeaponDBMS : public Script_Weapon
{
private:

public:
	Script_WeaponDBMS() = default;
	Script_WeaponDBMS(SPtr<GameObject> owner);
	virtual ~Script_WeaponDBMS();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

};

