#include "pch.h"
#include "Script_MiningMech.h"
#include "Animation.h"


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

void Script_MiningMech::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_MiningMech> otherScript = std::static_pointer_cast<Script_MiningMech>(other);

}

void Script_MiningMech::Activate()
{
    Script_Enemy::Activate();

}

void Script_MiningMech::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_MiningMech::WakeUp()
{
    Script_Enemy::WakeUp();

    return true;
}

bool Script_MiningMech::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(      &Script_MiningMech::DiggerAttackCallback,       this), 15);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack2AnimName())->AddCallback(std::bind(      &Script_MiningMech::DrillAttackCallback,        this), 15);

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack3AnimName())->AddStartCallback(std::bind( &Script_MiningMech::SmashAttackCallback,        this));
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack3AnimName())->AddCallback(std::bind(      &Script_MiningMech::SmashAttackCallback,        this), 20);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack3AnimName())->AddCallback(std::bind(      &Script_MiningMech::SmashAttackEndCallback,     this), 49);
    
    return true;
}

bool Script_MiningMech::Update()
{
    Script_Enemy::Update();

    return true;
}

void Script_MiningMech::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_MiningMech::Attack()
{
    Script_Enemy::Attack();

    Script_Enemy::mEnemyController->RemoveAllAnimation();
    GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackCnt);
    GetOwner()->GetAnimation()->GetController()->SetValue("IsAttack", true);


}

void Script_MiningMech::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_MiningMech::Dead()
{
    Script_Enemy::Dead();

}

bool Script_MiningMech::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return true;
}

/// +------------------------------------------------------------------
///             DIGGER ATTACK CALLBACK
/// ------------------------------------------------------------------+ 
void Script_MiningMech::DiggerAttackStartCallback()
{
}

void Script_MiningMech::DiggerAttackCallback()
{
}

void Script_MiningMech::DiggerAttackEndCallback()
{
}
/// +------------------------------------------------------------------
///             DRILL ATTACK CALLBACK
/// ------------------------------------------------------------------+ 

void Script_MiningMech::DrillAttackStartCallback()
{
}

void Script_MiningMech::DrillAttackCallback()
{
}

void Script_MiningMech::DrillAttackEndCallback()
{
}
/// +------------------------------------------------------------------
///             SMASH ATTACK CALLBACK
/// ------------------------------------------------------------------+ 

void Script_MiningMech::SmashAttackStartCallback()
{
}

void Script_MiningMech::SmashAttackCallback()
{

}

void Script_MiningMech::SmashAttackEndCallback()
{
}
/// +------------------------------------------------------------------
///             ATTACK END CALLBACK
/// ------------------------------------------------------------------+ 

void Script_MiningMech::AttackEndCallback()
{
    ++mCurrAttackCnt;
    mCurrAttackCnt %= AttackTypeCount;

    GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackCnt, true);
    GetOwner()->GetAnimation()->GetController()->SetValue("IsAttack", false);
    Script_Enemy::mEnemyController->SetState(EnemyInfo::State::Idle);

}
