#pragma once
#include "Script_Enemy.h"

class GameObject;


class Script_Aranobot : public Script_Enemy
{
private:

public:
	Script_Aranobot();
	Script_Aranobot(SPtr<GameObject> owner);
	virtual ~Script_Aranobot();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;	
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

};
