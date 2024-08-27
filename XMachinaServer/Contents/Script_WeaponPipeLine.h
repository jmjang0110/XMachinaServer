#pragma once
#include "Script_Weapon.h"

class GameObject;
class Script_WeaponPipeLine : public Script_Weapon
{
private:

public:
	Script_WeaponPipeLine() = default;
	Script_WeaponPipeLine(SPtr<GameObject> owner);
	virtual ~Script_WeaponPipeLine();


	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();

};

