#pragma once
#include "Script_Bullet.h"

class Script_RayCheckBullet : public Script_Bullet
{
private:
	int mRayCheckTargetID = -1;

public:
	Script_RayCheckBullet() = default;
	Script_RayCheckBullet(SPtr<GameObject> owner);
	virtual ~Script_RayCheckBullet();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Update();
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);

public:
	void SetRayCheckTargetID(int id) { mRayCheckTargetID = id; }
	void RayCheck();

};

