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
	Script_MiningMech(SPtr<GameObject> owner, ScriptInfo::Type type);
	~Script_MiningMech();

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
