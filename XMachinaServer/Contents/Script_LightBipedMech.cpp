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
    Script_EnemyStat::SetStat_AttackAnimName("DodgeForward");
    Script_EnemyStat::SetStat_DeathAnimName("Deactivation");
    Script_EnemyStat::SetStat_GetHitName("IdleActive");



}

Script_LightBipedMech::~Script_LightBipedMech()
{
}

void Script_LightBipedMech::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_LightBipedMech> otherScript = std::static_pointer_cast<Script_LightBipedMech>(other);

}

void Script_LightBipedMech::Activate()
{
}

void Script_LightBipedMech::DeActivate()
{
}

bool Script_LightBipedMech::WakeUp()
{
    return false;
}

bool Script_LightBipedMech::Start()
{
    return false;
}

bool Script_LightBipedMech::Update()
{
    return false;
}

void Script_LightBipedMech::OnDestroy()
{
}

void Script_LightBipedMech::Attack()
{
}

void Script_LightBipedMech::AttackCallback()
{
}

void Script_LightBipedMech::Dead()
{
}

bool Script_LightBipedMech::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
