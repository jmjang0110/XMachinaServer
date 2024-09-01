#include "pch.h"
#include "Script_EnemyController.h"
#include "Script_Stat.h"

#include "NPCController.h"
#include "GameRoom.h"
#include "Animation.h"
#include "GameObject.h"


Script_EnemyController::Script_EnemyController()
	: Script()
{
    mKey = uint32_t_ScriptKey(ScriptKey::EnemyController);
}

Script_EnemyController::Script_EnemyController(SPtr<GameObject> owner)
	: Script(owner, uint32_t_ScriptKey(ScriptKey::EnemyController))
{

}

Script_EnemyController::~Script_EnemyController()
{

}

SPtr<Component> Script_EnemyController::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_EnemyController
    auto clonedScript = std::dynamic_pointer_cast<Script_EnemyController>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_EnemyController" << std::endl;
        return nullptr;
    }
}

void Script_EnemyController::Clone(SPtr<GameObject> target)
{
    // Add a new Script_EnemyController to the target GameObject
    auto clonedScript = target->AddScript<Script_EnemyController>();
    // Use the component-based Clone method to copy the data
    this->Clone(clonedScript);
}

void Script_EnemyController::Start()
{
    mOwnerNPCController = mOwner->GetOwnerRoom()->GetNPCController();
    mOwnerRoom          = mOwner->GetOwnerRoom().get();
}


void Script_EnemyController::RemoveAllAnimation()
{

	OwnerAnimation()->GetController()->SetValue("Attack", 0);
	OwnerAnimation()->GetController()->SetValue("Walk", false);
	OwnerAnimation()->GetController()->SetValue("Return", false);
	OwnerAnimation()->GetController()->SetValue("GetHit", false);

}
void Script_EnemyController::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
}


void Script_EnemyController::Reset()
{
    mTarget     = nullptr;
    mPrevTarget = nullptr;

	while (!mPaths.empty()) {
		mPaths.pop();
	};

	RemoveAllAnimation();

}