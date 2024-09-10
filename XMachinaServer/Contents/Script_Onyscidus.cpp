#include "pch.h"
#include "Script_Onyscidus.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_BehaviorTrees.h"


#include "Script_BehaviorTrees.h"
Script_Onyscidus::Script_Onyscidus()
{
    mType = FBProtocol::MONSTER_TYPE_ONYSCIDUS;

}

Script_Onyscidus::Script_Onyscidus(SPtr<GameObject> owner)
	: Script_Enemy(owner)
{
    std::string name = "Onyscidus";
    mType = FBProtocol::MONSTER_TYPE_ONYSCIDUS;
    owner->SetName(name);
    Script_EnemyStat::SetDataFromDataBase(name);
    Script_EnemyStat::SetID(owner->GetID());
}

Script_Onyscidus::~Script_Onyscidus()
{
}

SPtr<Component> Script_Onyscidus::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Onyscidus>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Onyscidus" << std::endl;
        return nullptr;
    }
}

void Script_Onyscidus::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Onyscidus>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Onyscidus>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_Onyscidus::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();

    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Onyscidus::AttackCallback, this), 17);
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Onyscidus::AttackCallback, this), 44);

}

void Script_Onyscidus::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
