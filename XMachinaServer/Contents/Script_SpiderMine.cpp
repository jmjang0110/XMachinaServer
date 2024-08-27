#include "pch.h"
#include "Script_SpiderMine.h"
#include "GameObject.h"
#include "Component.h"

Script_SpiderMine::Script_SpiderMine()
	: Script_ExplosiveBullet()

{
}

Script_SpiderMine::Script_SpiderMine(SPtr<GameObject> owner)
	: Script_ExplosiveBullet(owner)
{
}

Script_SpiderMine::~Script_SpiderMine()
{
}

SPtr<Component> Script_SpiderMine::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_AdvancedCombatDroid_5
    auto clonedScript = std::dynamic_pointer_cast<Script_SpiderMine>(target);
    if (clonedScript)
    {
		clonedScript->mMass           = this->mMass;
		clonedScript->mDrag           = this->mDrag;
		clonedScript->mRotationSpeed  = this->mRotationSpeed;;
		clonedScript->mRotationX      = this->mRotationX;
		clonedScript->mRotationXSpeed = this->mRotationXSpeed;
		clonedScript->mPlantY         = this->mPlantY;
		clonedScript->mIsPlanted      = this->mIsPlanted;

		Script_ExplosiveBullet::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_SpiderMine" << std::endl;
        return nullptr;
    }
}
void Script_SpiderMine::Clone(SPtr<GameObject> target)
{
	auto clonedScript = target->AddScript<Script_SpiderMine>();
	this->Clone(clonedScript);
	clonedScript->SetOwner(target);
 }

void Script_SpiderMine::Start()
{
}

void Script_SpiderMine::Update()
{
}

void Script_SpiderMine::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}
