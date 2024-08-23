#pragma once

#include "Script_Enemy.h"

class GameObject;

class Script_Anglerox : public Script_Enemy
{
public:
	Script_Anglerox();
	Script_Anglerox(SPtr<GameObject> owner);
	virtual ~Script_Anglerox();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

};


