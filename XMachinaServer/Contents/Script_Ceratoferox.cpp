#include "pch.h"
#include "Script_Ceratoferox.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script_BehaviorTrees.h"

Script_Ceratoferox::Script_Ceratoferox()
{
    mType = FBProtocol::MONSTER_TYPE_CERATOFEROX;

}

Script_Ceratoferox::Script_Ceratoferox(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Ceratoferox";
    mType = FBProtocol::MONSTER_TYPE_CERATOFEROX;
    owner->SetName(name);
    Script_EnemyStat::SetDataFromDataBase(name);
    Script_EnemyStat::SetID(owner->GetID());
}

Script_Ceratoferox::~Script_Ceratoferox()
{
}

SPtr<Component> Script_Ceratoferox::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Ceratoferox>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Ceratoferox" << std::endl;
        return nullptr;
    }
}

void Script_Ceratoferox::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Ceratoferox>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Ceratoferox>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_Ceratoferox::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Ceratoferox::AttackCallback, this), 34);

}

void Script_Ceratoferox::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
