#include "pch.h"
#include "Script_Anglerox.h"

Script_Anglerox::Script_Anglerox()
{
}

Script_Anglerox::Script_Anglerox(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(5);
    Script_EnemyStat::SetStat_PheroLevel(4);
    Script_EnemyStat::SetStat_MoveSpeed(7.f);
    Script_EnemyStat::SetStat_DetectionRange(10);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(40);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(200);
    Script_EnemyStat::SetStat_Attack1AnimName("ClawsAttack2HitCombo");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");

    owner->SetName("Anglerox");


}

Script_Anglerox::~Script_Anglerox()
{
}

void Script_Anglerox::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Anglerox> otherScript = std::static_pointer_cast<Script_Anglerox>(other);

}

void Script_Anglerox::Activate()
{
}

void Script_Anglerox::DeActivate()
{
}

bool Script_Anglerox::WakeUp()
{
	return false;
}

bool Script_Anglerox::Start()
{
	return false;
}

bool Script_Anglerox::Update()
{
	return false;
}

void Script_Anglerox::OnDestroy()
{
}

void Script_Anglerox::AttackCallback()
{
}

void Script_Anglerox::Dead()
{
}

bool Script_Anglerox::Hit(float damage, SPtr_GameObject instigator)
{
	return false;
}
