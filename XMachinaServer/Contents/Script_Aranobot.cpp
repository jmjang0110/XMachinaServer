#include "pch.h"
#include "Script_Aranobot.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script_BehaviorTrees.h"

Script_Aranobot::Script_Aranobot()
    :Script_Enemy()
{
    mType = FBProtocol::MONSTER_TYPE_ARANOBOT;

}

Script_Aranobot::Script_Aranobot(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Aranobot";
    mType = FBProtocol::MONSTER_TYPE_ARANOBOT;
    owner->SetName(name);
    Script_EnemyStat::SetDataFromDataBase(name);
    Script_EnemyStat::SetID(owner->GetID());
}

Script_Aranobot::~Script_Aranobot()
{
}

SPtr<Component> Script_Aranobot::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Aranobot>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Aranobot" << std::endl;
        return nullptr;
    }
}

void Script_Aranobot::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Aranobot>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Aranobot>(clonedScript));

    clonedScript->SetOwner(target);
}


void Script_Aranobot::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Aranobot::AttackCallback, this), 6);

}

void Script_Aranobot::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
