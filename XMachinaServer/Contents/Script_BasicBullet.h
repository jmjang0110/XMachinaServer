#pragma once
#include "Script_Bullet.h"

class GameObject;
class Script_BasicBullet : public Script_Bullet
{
public:
	Script_BasicBullet();
	Script_BasicBullet(SPtr<GameObject> owner);
	virtual ~Script_BasicBullet();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Update();
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);

};

