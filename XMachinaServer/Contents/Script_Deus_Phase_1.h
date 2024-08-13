#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Deus_Phase_1 : public Script_Enemy
{
private:
	static constexpr float mkMeleeAttackRange = 3.f;
	static constexpr float mkExplodeAttackRate = 30.f;

protected:
	struct ScriptDeusPhase1AttackType : AttackType {
		static constexpr int MeleeAttack = 1;
		static constexpr int RangeAttack = 2;
		static constexpr int ExplodeAttack = 3;

		static constexpr int _count = 4;
	};

public:
	Script_Deus_Phase_1(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_Deus_Phase_1() = default;

public:
	virtual bool Start() override;
	virtual void StartAttack() override;

protected:
	void MeleeAttack();
	void RangeAttack();

	//void SmashAttackCallback();
	//void AttackEndCallback();
};
