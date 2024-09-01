#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Ceratoferox : public Script_Enemy
{
private:

public:
	Script_Ceratoferox();
	Script_Ceratoferox(SPtr<GameObject> owner);
	virtual ~Script_Ceratoferox();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
};
