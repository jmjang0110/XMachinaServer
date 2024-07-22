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
    Script_EnemyStat::SetStat_AttackAnimName("Direct2HitComboAttackForward");
    Script_EnemyStat::SetStat_DeathAnimName("Deactivation");
    Script_EnemyStat::SetStat_GetHitName("Blocked");



}

Script_MiningMech::~Script_MiningMech()
{
}

void Script_MiningMech::Clone(SPtr<Component> other)
{
}

void Script_MiningMech::Activate()
{
}

void Script_MiningMech::DeActivate()
{
}

bool Script_MiningMech::WakeUp()
{
    return false;
}

bool Script_MiningMech::Start()
{
    return false;
}

bool Script_MiningMech::Update()
{
    return false;
}

void Script_MiningMech::OnDestroy()
{
}

void Script_MiningMech::Attack()
{
}

void Script_MiningMech::AttackCallback()
{
}

void Script_MiningMech::Dead()
{
}

bool Script_MiningMech::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
