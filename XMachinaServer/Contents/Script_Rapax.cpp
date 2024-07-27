#include "pch.h"
#include "Script_Rapax.h"

Script_Rapax::Script_Rapax()
{
}

Script_Rapax::Script_Rapax(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(6);
    Script_EnemyStat::SetStat_PheroLevel(6);
    Script_EnemyStat::SetStat_MoveSpeed(4.5);
    Script_EnemyStat::SetStat_DetectionRange(30);
    Script_EnemyStat::SetStat_RotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(200);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(300);
    Script_EnemyStat::SetStat_Attack1AnimName("JumpClawsAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");


    owner->SetName("Rapax");

}

Script_Rapax::~Script_Rapax()
{
}

void Script_Rapax::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Rapax> otherScript = std::static_pointer_cast<Script_Rapax>(other);

}

void Script_Rapax::Activate()
{
    Script_Enemy::Activate();

}

void Script_Rapax::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_Rapax::WakeUp()
{
    Script_Enemy::WakeUp();

	return true;
}

bool Script_Rapax::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Rapax::AttackCallback, this), 35);

	return true;
}

bool Script_Rapax::Update()
{
    Script_Enemy::Update();

	return true;
}

void Script_Rapax::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Rapax::Attack()
{
    Script_Enemy::Attack();

}

void Script_Rapax::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_Rapax::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Rapax::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

	return false;
}
