#include "pch.h"
#include "Script_Gobbler.h"

Script_Gobbler::Script_Gobbler()
{
}

Script_Gobbler::Script_Gobbler(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(5);
    Script_EnemyStat::SetStat_PheroLevel(5);
    Script_EnemyStat::SetStat_MoveSpeed(3);
    Script_EnemyStat::SetStat_DetectionRange(25);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(100);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(500);
    Script_EnemyStat::SetStat_AttackAnimName("RamAttack");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("Blocked");



}
Script_Gobbler::~Script_Gobbler()
{
}

void Script_Gobbler::Clone(SPtr<Component> other)
{
}

void Script_Gobbler::Activate()
{
}

void Script_Gobbler::DeActivate()
{
}

bool Script_Gobbler::WakeUp()
{
    return false;
}

bool Script_Gobbler::Start()
{
    return false;
}

bool Script_Gobbler::Update()
{
    return false;
}

void Script_Gobbler::OnDestroy()
{
}

void Script_Gobbler::Attack()
{
}

void Script_Gobbler::AttackCallback()
{
}

void Script_Gobbler::Dead()
{
}

bool Script_Gobbler::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
