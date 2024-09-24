#pragma once
#include "Script_ExplosiveBullet.h"

class GameObject;
class Script_BulletAirStrike : public Script_ExplosiveBullet
{
public:
	Script_BulletAirStrike() = default;
	Script_BulletAirStrike(SPtr<GameObject> owner);
	virtual ~Script_BulletAirStrike();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);

	virtual void Update() override;

	virtual void SplashDamage() override;
	virtual void Explode() override;

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);

};

