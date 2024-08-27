#pragma once
#include "Script_Enemy.h"

class GameObject;
class AnimatorController;
class Script_Ursacetus : public Script_Enemy
{
protected:
	struct UrsacetusAttackType : AttackType {
		static constexpr int RoarAttack    = 4;
		static constexpr int SpecialAttack = 5;
		static constexpr int _count        = 6;
	};

private:
	SPtr<AnimatorController> mAnimController;

public:
	Script_Ursacetus();
	Script_Ursacetus(SPtr<GameObject> owner);
	virtual ~Script_Ursacetus();

public:
	virtual SPtr<Component> Clone(SPtr<Component> target);
	virtual void Clone(SPtr<GameObject> target);

	virtual void Start() override;
	virtual void Dispatch(class OverlappedObject* overlapped, UINT32 bytes = 0);

protected:
	void BasicAttackCallback();
	void SpecialAttackCallback();

	virtual void AttackEndCallback() override;
};

