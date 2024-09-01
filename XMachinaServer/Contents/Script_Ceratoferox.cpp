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
    mType = FBProtocol::MONSTER_TYPE_CERATOFEROX;

    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(3);
    Script_EnemyStat::SetStat_PheroLevel(3);
    Script_EnemyStat::SetStat_MoveSpeed(1.5);
    Script_EnemyStat::SetStat_DetectionRange(10);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(50);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(200);
    Script_EnemyStat::SetStat_Attack1AnimName("DoubleClawsAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_GetHitName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");


    owner->SetName("Ceratoferox");
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
