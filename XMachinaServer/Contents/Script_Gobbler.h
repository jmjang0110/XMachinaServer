#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Gobbler : public Script_Enemy
{
public:
	Script_Gobbler();
	Script_Gobbler(SPtr<GameObject> owner);
	virtual ~Script_Gobbler();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
};
