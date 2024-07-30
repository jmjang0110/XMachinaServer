#include "pch.h"
#include "Script_Onyscidus.h"

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
    Script_EnemyStat::SetMaxHP(70);
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
}

void Script_Onyscidus::DeActivate()
{
}

bool Script_Onyscidus::WakeUp()
{
	return false;
}

bool Script_Onyscidus::Start()
{
	return false;
}

bool Script_Onyscidus::Update()
{
	return false;
}

void Script_Onyscidus::OnDestroy()
{
}

void Script_Onyscidus::AttackCallback()
{
}

void Script_Onyscidus::Dead()
{
}

bool Script_Onyscidus::Hit(float damage, SPtr_GameObject instigator)
{
	return false;
}
