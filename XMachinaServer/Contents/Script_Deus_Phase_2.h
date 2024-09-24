#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Deus_Phase_2 : public Script_Enemy
{
private:
	static constexpr float mkMeleeAttackRange  = 3.f;
	static constexpr float mkExplodeAttackRate = 30.f;

protected:
	struct ScriptDeusPhase2AttackType : AttackType {
		static constexpr int MeleeAttack   = 1;
		static constexpr int RangeAttack   = 2;
		static constexpr int ExplodeAttack = 3;

		static constexpr int _count        = 4;
	};

public:
	Script_Deus_Phase_2(SPtr<GameObject> owner);
	virtual ~Script_Deus_Phase_2() = default;

	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

public:
	virtual void Start() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0) override;
};
