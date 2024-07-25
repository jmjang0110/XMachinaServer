#include "pch.h"
#include "Script_Arack.h"

Script_Arack::Script_Arack()
{
}

Script_Arack::Script_Arack(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(1);
    Script_EnemyStat::SetStat_PheroLevel(1);
    Script_EnemyStat::SetStat_MoveSpeed(4.5);
    Script_EnemyStat::SetStat_DetectionRange(10);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(5);
    Script_EnemyStat::SetStat_AttackRange(3);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(50);
    Script_EnemyStat::SetStat_AttackAnimName("ClawsAttack");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");

    owner->SetName("Arack");


}
Script_Arack::~Script_Arack()
{
}

void Script_Arack::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Arack> otherScript = std::static_pointer_cast<Script_Arack>(other);

}

void Script_Arack::Activate()
{
}

void Script_Arack::DeActivate()
{
}

bool Script_Arack::WakeUp()
{
    return false;
}

bool Script_Arack::Start()
{
    return false;
}

bool Script_Arack::Update()
{
    return false;
}

void Script_Arack::OnDestroy()
{
}

void Script_Arack::Attack()
{
}

void Script_Arack::AttackCallback()
{
}

void Script_Arack::Dead()
{
}

bool Script_Arack::Hit(float damage, SPtr_GameObject instigator)
{
    return false;
}
