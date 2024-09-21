#include "pch.h"
#include "Script_LightBipedMech.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_BehaviorTrees.h"
Script_LightBipedMech::Script_LightBipedMech()
{
}

Script_LightBipedMech::Script_LightBipedMech(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "LightBipedMech";
    mType = FBProtocol::MONSTER_TYPE_LIGHTBIPEDMECH;
    owner->SetName(name);
#ifdef SET_DATA_FROM_DATABASE
    Script_EnemyStat::SetDataFromDataBase(name);
#else
    Script_EnemyStat::SetStat_EnemyLevel(8);
    Script_EnemyStat::SetStat_PheroLevel(7);
    Script_EnemyStat::SetStat_MoveSpeed(2.5);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(250);
    Script_EnemyStat::SetStat_AttackRange(6);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(600);
    Script_EnemyStat::SetStat_Attack1AnimName("DodgeForward");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Deactivation");
    Script_EnemyStat::SetStat_GetHitName("IdleActive");
#endif
    Script_EnemyStat::SetID(owner->GetID());
}

Script_LightBipedMech::~Script_LightBipedMech()
{
}

SPtr<Component> Script_LightBipedMech::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_LightBipedMech>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_LightBipedMech" << std::endl;
        return nullptr;
    }
}

void Script_LightBipedMech::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_LightBipedMech>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_LightBipedMech>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_LightBipedMech::Start()
{
    Script_Enemy::Start();
    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_LightBipedMech::AttackCallback, this), 20);

}

void Script_LightBipedMech::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
