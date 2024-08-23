#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Arack : public Script_Enemy
{
private:
public:
	Script_Arack();
	Script_Arack(SPtr<GameObject> owner);
	virtual ~Script_Arack();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);


	virtual void Start();
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;

};
