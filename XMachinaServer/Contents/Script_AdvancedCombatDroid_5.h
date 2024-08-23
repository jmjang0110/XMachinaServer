#pragma once
#include "Script_Enemy.h"

class GameObject;


class Script_AdvancedCombatDroid_5 : public Script_Enemy
{
public:
	Script_AdvancedCombatDroid_5();
	Script_AdvancedCombatDroid_5(SPtr<GameObject> owner);
	virtual ~Script_AdvancedCombatDroid_5();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start();


	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
};

