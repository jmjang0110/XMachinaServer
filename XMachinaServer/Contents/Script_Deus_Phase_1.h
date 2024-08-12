#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Deus_Phase_1 : public Script_Enemy
{
protected:
	struct ScriptDeusPhase1AttackType : AttackType {
		static constexpr int MeleeAttack = 1;
		static constexpr int RangeAttack = 2;
		static constexpr int ExplodeAttack = 3;

		static constexpr int _count = 4;
	};

public:
	Script_Deus_Phase_1();
	Script_Deus_Phase_1(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_Deus_Phase_1();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual bool Start() override;
	virtual void Clone(SPtr<Component> other);

protected:
	void SmashAttackCallback();
	void AttackEndCallback();
};
