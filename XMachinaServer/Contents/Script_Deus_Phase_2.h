#pragma once

#include "Script_Enemy.h"

class GameObject;
class Script_Deus_Phase_2 : public Script_Enemy
{
protected:
	struct ScriptDeusPhase2AttackType : AttackType {
		static constexpr int DiggerAttack = 1;
		static constexpr int DrillAttack = 2;
		static constexpr int SmashAttack = 3;

		static constexpr int _count = 4;
	};

public:
	Script_Deus_Phase_2();
	Script_Deus_Phase_2(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_Deus_Phase_2();

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
