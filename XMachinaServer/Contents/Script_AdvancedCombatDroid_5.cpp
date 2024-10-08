#include "pch.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_BehaviorTrees.h"

#include "GameObject.h"

Script_AdvancedCombatDroid_5::Script_AdvancedCombatDroid_5()
{
    mType = FBProtocol::MONSTER_TYPE_ADVANCED_COMBAT_DROIR_5;

}

Script_AdvancedCombatDroid_5::Script_AdvancedCombatDroid_5(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
   std::string name = "AdvancedCombatDroid_5";
   mType            = FBProtocol::MONSTER_TYPE_ADVANCED_COMBAT_DROIR_5;
   owner->SetName(name);
#ifdef SET_DATA_FROM_DATABASE
   Script_EnemyStat::SetDataFromDataBase(name);
#else
   Script_EnemyStat::SetStat_EnemyLevel(3);
   Script_EnemyStat::SetStat_PheroLevel(3);
   Script_EnemyStat::SetStat_MoveSpeed(2.5);
   Script_EnemyStat::SetStat_DetectionRange(15);
   Script_EnemyStat::SetStat_RotationSpeed(150);
   Script_EnemyStat::SetStat_AttackRotationSpeed(250);
   Script_EnemyStat::SetStat_AttackRate(30);
   Script_EnemyStat::SetStat_AttackRange(1.5);
   Script_EnemyStat::SetStat_AttackCoolTime(0);
   Script_EnemyStat::SetMaxHP(150);
   Script_EnemyStat::SetStat_Attack1AnimName("2HitComboUnarmed");
   Script_EnemyStat::SetStat_Attack2AnimName("None");
   Script_EnemyStat::SetStat_Attack3AnimName("None");
   Script_EnemyStat::SetStat_DeathAnimName("DeathFrontCombat");
   Script_EnemyStat::SetStat_GetHitName("IdleCombat");
#endif
   Script_EnemyStat::SetID(owner->GetID());
}

Script_AdvancedCombatDroid_5::~Script_AdvancedCombatDroid_5()
{
}

SPtr<Component> Script_AdvancedCombatDroid_5::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_AdvancedCombatDroid_5
    auto clonedScript = std::dynamic_pointer_cast<Script_AdvancedCombatDroid_5>(target);
    if (clonedScript)
    {
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_AdvancedCombatDroid_5" << std::endl;
        return nullptr;
    }
}

void Script_AdvancedCombatDroid_5::Clone(SPtr<GameObject> target)
{
    // Add a new Script_AdvancedCombatDroid_5 instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_AdvancedCombatDroid_5>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_AdvancedCombatDroid_5>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_AdvancedCombatDroid_5::Start()
{
    Script_Enemy::Start();
 

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_AdvancedCombatDroid_5::AttackCallback, this), 3);
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_AdvancedCombatDroid_5::AttackCallback, this), 20);

}

void Script_AdvancedCombatDroid_5::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
