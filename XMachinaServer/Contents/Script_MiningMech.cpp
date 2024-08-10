#include "pch.h"
#include "Script_MiningMech.h"

Script_MiningMech::Script_MiningMech()
{
}

Script_MiningMech::Script_MiningMech(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(7);
    Script_EnemyStat::SetStat_PheroLevel(6);
    Script_EnemyStat::SetStat_MoveSpeed(5.5);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(150);
    Script_EnemyStat::SetStat_AttackRotationSpeed(100);
    Script_EnemyStat::SetStat_AttackRate(300);
    Script_EnemyStat::SetStat_AttackRange(5);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(1500);
    Script_EnemyStat::SetStat_Attack1AnimName("UppercutDiggerAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("UppercutDrillAttack");
    Script_EnemyStat::SetStat_Attack3AnimName("SmashAttack");
    Script_EnemyStat::SetStat_DeathAnimName("Deactivation");
    Script_EnemyStat::SetStat_GetHitName("Blocked");


    owner->SetName("MiningMech");

}

Script_MiningMech::~Script_MiningMech()
{
}

bool Script_MiningMech::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }


    return true;
}

void Script_MiningMech::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_MiningMech> otherScript = std::static_pointer_cast<Script_MiningMech>(other);

}

