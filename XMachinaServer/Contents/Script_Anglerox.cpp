#include "pch.h"
#include "Script_Anglerox.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script_BehaviorTrees.h"

Script_Anglerox::Script_Anglerox()
{
    mType = FBProtocol::MONSTER_TYPE_ANGLEROX;

}

Script_Anglerox::Script_Anglerox(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    mType = FBProtocol::MONSTER_TYPE_ANGLEROX;
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(5);
    Script_EnemyStat::SetStat_PheroLevel(4);
    Script_EnemyStat::SetStat_MoveSpeed(7.f);
    Script_EnemyStat::SetStat_DetectionRange(10);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(40);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(200);
    Script_EnemyStat::SetStat_Attack1AnimName("ClawsAttack2HitCombo");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");

    owner->SetName("Anglerox");

}

Script_Anglerox::~Script_Anglerox()
{
}
SPtr<Component> Script_Anglerox::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Anglerox>(target);
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

void Script_Anglerox::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Anglerox>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Anglerox>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_Anglerox::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();

    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Anglerox::AttackCallback, this), 10);
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Anglerox::AttackCallback, this), 32);

}

void Script_Anglerox::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
