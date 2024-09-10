#include "pch.h"
#include "Script_Gobbler.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script_BehaviorTrees.h"

Script_Gobbler::Script_Gobbler()
{
    mType = FBProtocol::MONSTER_TYPE_GOBBLER;

}

Script_Gobbler::Script_Gobbler(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Gobbler";
    mType = FBProtocol::MONSTER_TYPE_GOBBLER;
    owner->SetName(name);
    Script_EnemyStat::SetDataFromDataBase(name);
    Script_EnemyStat::SetID(owner->GetID());
}
Script_Gobbler::~Script_Gobbler()
{
}

SPtr<Component> Script_Gobbler::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Gobbler>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Anglerox" << std::endl;
        return nullptr;
    }
}

void Script_Gobbler::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Gobbler>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Gobbler>(clonedScript));

    clonedScript->SetOwner(target);
}
void Script_Gobbler::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Gobbler::AttackCallback, this), 20);

}

void Script_Gobbler::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
    MEMORY->Delete(overlapped);

    int activeReference = mOwner->GetActivateRef();
    if (activeReference > 0) {
        mOwner->Update();
        mOwner->RegisterUpdate();
    }
    else {
        OnExitFromViewList();
    }


}
