#pragma once
#include "Script_Weapon.h"

class GameObject;
class Script_WeaponSkyLine : public Script_Weapon
{
private:

public:
	Script_WeaponSkyLine() = default;
	Script_WeaponSkyLine(SPtr<GameObject> owner);
	virtual ~Script_WeaponSkyLine();

public:

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();

};

