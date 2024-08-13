#include "pch.h"
#include "Script_LightBipedMech.h"

Script_LightBipedMech::Script_LightBipedMech()
{
}

Script_LightBipedMech::Script_LightBipedMech(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

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


    owner->SetName("LightBipedMech");

}

Script_LightBipedMech::~Script_LightBipedMech()
{
}

bool Script_LightBipedMech::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_LightBipedMech::AttackCallback, this), 20);

    return true;
}