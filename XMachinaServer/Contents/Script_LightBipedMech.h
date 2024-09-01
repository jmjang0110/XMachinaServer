#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_LightBipedMech : public Script_Enemy
{
private:


public:
	Script_LightBipedMech();
	Script_LightBipedMech(SPtr<GameObject> owner);
	virtual ~Script_LightBipedMech();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
};
