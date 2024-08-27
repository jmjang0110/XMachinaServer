#pragma once
#include "Script_Bullet.h"

class GameObject;
class Script_ExplosiveBullet : public Script_Bullet
{
private:
	float mExplosionDamage = {};

public:
	Script_ExplosiveBullet();
	Script_ExplosiveBullet(SPtr<GameObject> owner);
	virtual ~Script_ExplosiveBullet();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);

public:
	void SetExplosionDamage(float damage) { mExplosionDamage = damage; }

};

