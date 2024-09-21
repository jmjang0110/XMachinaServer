#pragma once
#include "Script_Weapon.h"
class GameObject;

class Script_WeaponDeus_Phase_1_Rifle : public Script_Weapon
{
private:

public:
	Script_WeaponDeus_Phase_1_Rifle() = default;
	Script_WeaponDeus_Phase_1_Rifle(SPtr<GameObject> owner);
	virtual ~Script_WeaponDeus_Phase_1_Rifle();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();



};
