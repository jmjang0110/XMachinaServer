#pragma once

#include "Script_Weapon.h"

class GameObject;
class Script_WeaponAirStrike : public Script_Weapon
{
private:

public:
	Script_WeaponAirStrike() = default;
	Script_WeaponAirStrike(SPtr<GameObject> owner);
	virtual ~Script_WeaponAirStrike();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();

};

