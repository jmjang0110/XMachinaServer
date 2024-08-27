#include "pch.h"
#include "Script_ExplosiveBullet.h"
#include "GameObject.h"

Script_ExplosiveBullet::Script_ExplosiveBullet()
	: Script_Bullet()
{
}

Script_ExplosiveBullet::Script_ExplosiveBullet(SPtr<GameObject> owner)
	: Script_Bullet(owner)
{
}

Script_ExplosiveBullet::~Script_ExplosiveBullet()
{
}

SPtr<Component> Script_ExplosiveBullet::Clone(SPtr<Component> target)
{
	// Try to cast the target to Script_AdvancedCombatDroid_5
	auto clonedScript = std::dynamic_pointer_cast<Script_ExplosiveBullet>(target);
	if (clonedScript)
	{
		Script_Bullet::Clone(clonedScript);
		clonedScript->mExplosionDamage = this->mExplosionDamage;
		return clonedScript;
	}
	else
	{
		std::cout << "Clone failed: target is not of type Script_ExplosiveBullet" << std::endl;
		return nullptr;
	}
}

