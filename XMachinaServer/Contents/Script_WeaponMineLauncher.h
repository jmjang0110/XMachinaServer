#pragma once
#include "Script_Weapon.h"

class GameObject;
class Script_WeaponMineLauncher : public Script_Weapon
{
private:

public:
	Script_WeaponMineLauncher() = default;
	Script_WeaponMineLauncher(SPtr<GameObject> owner);
	virtual ~Script_WeaponMineLauncher();

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Update();

	virtual int	OnShoot(Vec3& center_pos, Vec3& fire_dir);

};

