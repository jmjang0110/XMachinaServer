#pragma once
#include "Script_Enemy.h"

class GameObject;
class AnimationController;

class Script_Ursacetus : public Script_Enemy
{
private:
	enum class AttackType {
		BasicAttack = 1,	
		RoarAttack = 4,		
		SpecialAttack = 5,

		_count,
	};
	enum { AttackTypeCount = static_cast<UINT8>(AttackType::_count) };

	int mCurrAttackCnt{};
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
	virtual bool LateUpdate() override;
	virtual bool Attack() override;

protected:
	void BasicAttackCallback();
	void SpecialAttackCallback();

	virtual void AttackEndCallback() override;
};

