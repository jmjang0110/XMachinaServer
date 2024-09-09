#pragma once
#include "Script_Bullet.h"

class GameObject;
class Script_ExplosiveBullet : public Script_Bullet
{
protected:
	float mSplashRangeRadius = {};
	float mExplosionDamage   = {};

public:
	Script_ExplosiveBullet();
	Script_ExplosiveBullet(SPtr<GameObject> owner);
	virtual ~Script_ExplosiveBullet();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void SplashDamage() = 0;

public:
	void SetExplosionDamage(float damage) { mExplosionDamage = damage; }

};

