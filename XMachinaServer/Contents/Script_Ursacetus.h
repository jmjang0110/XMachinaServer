#pragma once
#include "Script_Enemy.h"

class GameObject;
class AnimationController;

class Script_Ursacetus : public Script_Enemy
{
protected:
	struct UrsacetusAttackType : AttackType {
		static constexpr int RoarAttack = 4;
		static constexpr int SpecialAttack = 5;

		static constexpr int _count = 6;
	};

private:
	SPtr<AnimatorController> mAnimController;

public:
	Script_Ursacetus();
	Script_Ursacetus(SPtr<GameObject> owner, ScriptInfo::Type type);
	virtual ~Script_Ursacetus();

public:
	/// +------------------------------
	///		  virtual function 
	/// ------------------------------+
	virtual void Clone(SPtr<Component> other) ;
	/// +------------------------------
	///		Stat :  virtual function 
	/// ------------------------------+
	virtual bool Start() override;

protected:
	void BasicAttackCallback();
	void SpecialAttackCallback();

	virtual void AttackEndCallback() override;
};

