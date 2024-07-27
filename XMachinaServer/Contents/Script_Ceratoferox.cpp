#include "pch.h"
#include "Script_Ceratoferox.h"

Script_Ceratoferox::Script_Ceratoferox()
{
}

Script_Ceratoferox::Script_Ceratoferox(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(3);
    Script_EnemyStat::SetStat_PheroLevel(3);
    Script_EnemyStat::SetStat_MoveSpeed(1.5);
    Script_EnemyStat::SetStat_DetectionRange(10);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(50);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(300);
    Script_EnemyStat::SetStat_Attack1AnimName("DoubleClawsAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("GetHitFront");
    Script_EnemyStat::SetStat_GetHitName("Death");


    owner->SetName("Ceratoferox");

}

Script_Ceratoferox::~Script_Ceratoferox()
{

}

void Script_Ceratoferox::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Ceratoferox> otherScript = std::static_pointer_cast<Script_Ceratoferox>(other);

}

void Script_Ceratoferox::Activate()
{
    Script_Enemy::Activate();

}

void Script_Ceratoferox::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_Ceratoferox::WakeUp()
{
    Script_Enemy::WakeUp();

    return false;
}

bool Script_Ceratoferox::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Ceratoferox::AttackCallback, this), 34);
    
    return false;
}

bool Script_Ceratoferox::Update()
{
    Script_Enemy::Update();

    return false;
}

void Script_Ceratoferox::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Ceratoferox::Attack()
{
    Script_Enemy::Attack();

}

void Script_Ceratoferox::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_Ceratoferox::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Ceratoferox::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return false;
}
