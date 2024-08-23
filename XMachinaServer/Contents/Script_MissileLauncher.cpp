#include "pch.h"
#include "Script_MissileLauncher.h"
#include "GameObject.h"


Script_MissileLauncher::Script_MissileLauncher()
	: Script_Bullet()

{
}

Script_MissileLauncher::Script_MissileLauncher(SPtr<GameObject> owner)
	: Script_Bullet(owner)
{
}

Script_MissileLauncher::~Script_MissileLauncher()
{
}

SPtr<Component> Script_MissileLauncher::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_AdvancedCombatDroid_5
    auto clonedScript = std::dynamic_pointer_cast<Script_MissileLauncher>(target);
    if (clonedScript)
    {
		clonedScript->mMass           = this->mMass;
		clonedScript->mDrag           = this->mDrag;
		clonedScript->mRotationSpeed  = this->mRotationSpeed;;
		clonedScript->mRotationX      = this->mRotationX;
		clonedScript->mRotationXSpeed = this->mRotationXSpeed;
		clonedScript->mPlantY         = this->mPlantY;
		clonedScript->mIsPlanted      = this->mIsPlanted;

        Script_Bullet::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_MissileLauncher" << std::endl;
        return nullptr;
    }
}
void Script_MissileLauncher::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_MissileLauncher>();
	this->Clone(clonedScript);
	clonedScript->SetOwner(target);
 }
