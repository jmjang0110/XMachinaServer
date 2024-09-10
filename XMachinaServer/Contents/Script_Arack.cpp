#include "pch.h"
#include "Script_Arack.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_BehaviorTrees.h"
Script_Arack::Script_Arack()
{
    mType = FBProtocol::MONSTER_TYPE_ARACK;

}

Script_Arack::Script_Arack(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Arack";
    mType = FBProtocol::MONSTER_TYPE_ARACK;
    owner->SetName(name);
    Script_EnemyStat::SetDataFromDataBase(name);
    Script_EnemyStat::SetID(owner->GetID());
}
Script_Arack::~Script_Arack()
{
}

SPtr<Component> Script_Arack::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Arack>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Arack" << std::endl;
        return nullptr;
    }
}

void Script_Arack::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Arack>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Arack>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_Arack::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Arack::AttackCallback, this), 13);

}

void Script_Arack::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
