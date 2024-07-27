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

void Script_LightBipedMech::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_LightBipedMech> otherScript = std::static_pointer_cast<Script_LightBipedMech>(other);

}

void Script_LightBipedMech::Activate()
{
    Script_Enemy::Activate();

}

void Script_LightBipedMech::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_LightBipedMech::WakeUp()
{
    Script_Enemy::WakeUp();

    return false;
}

bool Script_LightBipedMech::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_LightBipedMech::AttackCallback, this), 20);

    return false;
}

bool Script_LightBipedMech::Update()
{
    Script_Enemy::Update();

    return false;
}

void Script_LightBipedMech::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_LightBipedMech::Attack()
{
    Script_Enemy::Attack();

}

void Script_LightBipedMech::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_LightBipedMech::Dead()
{
    Script_Enemy::Dead();

}

bool Script_LightBipedMech::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return false;
}
