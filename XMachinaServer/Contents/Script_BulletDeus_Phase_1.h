#pragma once

#include "Script_ExplosiveBullet.h"

class Script_BulletDeus_Phase_1 : public Script_ExplosiveBullet
{
private:

public:
	Script_BulletDeus_Phase_1();
	Script_BulletDeus_Phase_1(SPtr<GameObject> owner);
	virtual ~Script_BulletDeus_Phase_1();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);

	virtual void Update() override;

	virtual void SplashDamage() override;
	virtual void Explode() override;

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);
};

