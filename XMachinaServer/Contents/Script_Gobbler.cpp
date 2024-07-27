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
    Script_EnemyStat::SetStat_Attack1AnimName("RamAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Roar");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");

    owner->SetName("Gobbler");


}
Script_Gobbler::~Script_Gobbler()
{
}

void Script_Gobbler::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Gobbler> otherScript = std::static_pointer_cast<Script_Gobbler>(other);

}

void Script_Gobbler::Activate()
{
    Script_Enemy::Activate();

}

void Script_Gobbler::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_Gobbler::WakeUp()
{
    Script_Enemy::WakeUp();

    return false;
}

bool Script_Gobbler::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Gobbler::AttackCallback, this), 20);

    return false;
}

bool Script_Gobbler::Update()
{
    Script_Enemy::Update();

    return false;
}

void Script_Gobbler::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Gobbler::Attack()
{
    Script_Enemy::Attack();

}

void Script_Gobbler::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_Gobbler::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Gobbler::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return false;
}
