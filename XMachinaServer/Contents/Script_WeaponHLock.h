#pragma once
#include "Script_Weapon.h"
class GameObject;
class Script_WeaponHLock : public Script_Weapon
{
private:

public:
	Script_WeaponHLock() = default;
	Script_WeaponHLock(SPtr<GameObject> owner);
	virtual ~Script_WeaponHLock();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();



};


