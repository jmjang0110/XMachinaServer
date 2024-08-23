#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_MiningMech : public Script_Enemy
{
protected:
	struct MiningMechAttackType : AttackType {
		static constexpr int DiggerAttack = 1;
		static constexpr int DrillAttack = 2;
		static constexpr int SmashAttack = 3;

		static constexpr int _count = 4;
	};

public:
	Script_MiningMech();
	Script_MiningMech(SPtr<GameObject> owner);
	virtual ~Script_MiningMech();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;

protected:
	void SmashAttackCallback();
	void AttackEndCallback();
};
