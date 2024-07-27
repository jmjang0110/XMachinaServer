#include "pch.h"
#include "Script_Onyscidus.h"

#include "Animation.h"

Script_Onyscidus::Script_Onyscidus()
{
}

Script_Onyscidus::Script_Onyscidus(SPtr<GameObject> owner, ScriptInfo::Type type)
	: Script_Enemy(owner, type)
{
    Script_EnemyStat::SetStat_EnemyLevel(1);
    Script_EnemyStat::SetStat_PheroLevel(1);
    Script_EnemyStat::SetStat_MoveSpeed(2.5);
    Script_EnemyStat::SetStat_DetectionRange(15);
    Script_EnemyStat::SetStat_RotationSpeed(150);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(10);
    Script_EnemyStat::SetStat_AttackRange(4);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(100);
    Script_EnemyStat::SetStat_Attack1AnimName("2HitComboClawsAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("BlockIdle");

    owner->SetName("Onyscidus");


}

Script_Onyscidus::~Script_Onyscidus()
{
}

void Script_Onyscidus::Clone(SPtr<Component> other) 
{
	Script_Enemy::Clone(other);
	SPtr<Script_Onyscidus> otherScript = std::static_pointer_cast<Script_Onyscidus>(other);

}

void Script_Onyscidus::Activate()
{
    Script_Enemy::Activate();

}

void Script_Onyscidus::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_Onyscidus::WakeUp()
{
    Script_Enemy::WakeUp();

	return true;
}

bool Script_Onyscidus::Start()
{
    Script_Enemy::Start();

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Onyscidus::AttackCallback, this), 17);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Onyscidus::AttackCallback, this), 44);

	return true;
}

bool Script_Onyscidus::Update()
{
    Script_Enemy::Update();

	return true;
}

void Script_Onyscidus::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Onyscidus::Attack()
{
    Script_Enemy::Attack();

}

void Script_Onyscidus::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_Onyscidus::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Onyscidus::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

	return false;
}
