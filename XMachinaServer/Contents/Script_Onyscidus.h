#pragma once


#include "Script_Enemy.h"

class Component;
class GameObject;
class Script_Onyscidus : public Script_Enemy
{
private:
	
;public:
	Script_Onyscidus();
	Script_Onyscidus(SPtr<GameObject> owner);
	virtual ~Script_Onyscidus();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;

	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

};

