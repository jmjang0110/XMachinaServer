#include "pch.h"
#include "Script_AdvancedCombatDroid_5.h"

Script_AdvancedCombatDroid_5::Script_AdvancedCombatDroid_5()
{
}

Script_AdvancedCombatDroid_5::Script_AdvancedCombatDroid_5(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());
    
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

   owner->SetName("AdvancedCombatDroid_5");


}

Script_AdvancedCombatDroid_5::~Script_AdvancedCombatDroid_5()
{
}

bool Script_AdvancedCombatDroid_5::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_AdvancedCombatDroid_5::AttackCallback, this), 3);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_AdvancedCombatDroid_5::AttackCallback, this), 20);

    return true;
}

void Script_AdvancedCombatDroid_5::Clone(SPtr<Component> other) 
{
    Script_Enemy::Clone(other);
    SPtr<Script_AdvancedCombatDroid_5> otherScript = std::static_pointer_cast<Script_AdvancedCombatDroid_5>(other);

}

