#include "pch.h"
#include "Script_Ursacetus.h"
#include "GameObject.h"
Script_Ursacetus::Script_Ursacetus()
    : Script_Enemy()

{
}

Script_Ursacetus::Script_Ursacetus(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script_Enemy(owner,  type)
{
    Script_EnemyStat::SetStat_EnemyLevel(4);
    Script_EnemyStat::SetStat_PheroLevel(5);
    Script_EnemyStat::SetStat_MoveSpeed(5);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(150);
    Script_EnemyStat::SetStat_AttackRotationSpeed(10);
    Script_EnemyStat::SetStat_AttackRate(200);
    Script_EnemyStat::SetStat_AttackRange(5);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(1000);
    Script_EnemyStat::SetStat_Attack1AnimName("LeftHandAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("Roar1");
    Script_EnemyStat::SetStat_Attack3AnimName("LeftFootStompAttack");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("None");

    owner->SetName("Ursacetus");

}

Script_Ursacetus::~Script_Ursacetus()
{
}

void Script_Ursacetus::Clone(SPtr<Component> other) 
{
    Script_Enemy::Clone(other);
    SPtr<Script_Ursacetus> otherScript = std::static_pointer_cast<Script_Ursacetus>(other);

}

void Script_Ursacetus::Activate()
{
    Script_Enemy::Update();

}

void Script_Ursacetus::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_Ursacetus::WakeUp()
{
    Script_Enemy::WakeUp();

    return false;
}

bool Script_Ursacetus::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Ursacetus::AttackCallback, this), 3);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Ursacetus::AttackCallback, this), 20);

    return false;
}

bool Script_Ursacetus::Update()
{
    Script_Enemy::Update();

    return true;
}

void Script_Ursacetus::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Ursacetus::Attack()
{
    Script_Enemy::Attack();

}

void Script_Ursacetus::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_Ursacetus::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Ursacetus::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return false;
}

void Script_Ursacetus::BasicAttackCallback()
{
}

void Script_Ursacetus::RoarAttackCallback()
{
}

void Script_Ursacetus::SpecialAttack()
{
}

void Script_Ursacetus::SpecialAttackCallback()
{
}

void Script_Ursacetus::SpecialAttackStartCallback()
{
}

void Script_Ursacetus::AttackEndCallback()
{
}
