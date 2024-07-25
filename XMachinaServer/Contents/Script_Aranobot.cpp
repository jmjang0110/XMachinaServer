#include "pch.h"
#include "Script_Aranobot.h"

Script_Aranobot::Script_Aranobot()
    :Script_Enemy()
{
}

Script_Aranobot::Script_Aranobot(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(2);
    Script_EnemyStat::SetStat_PheroLevel(2);
    Script_EnemyStat::SetStat_MoveSpeed(2);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(20);
    Script_EnemyStat::SetStat_AttackRange(6);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(200);
    Script_EnemyStat::SetStat_AttackAnimName("GetHitFront");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("GetHitLeft");

    owner->SetName("Aranobot");


}

Script_Aranobot::~Script_Aranobot()
{
}

void Script_Aranobot::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Aranobot> otherScript = std::static_pointer_cast<Script_Aranobot>(other);

}

void Script_Aranobot::Activate()
{
}

void Script_Aranobot::DeActivate()
{
}

bool Script_Aranobot::WakeUp()
{
	return false;
}

bool Script_Aranobot::Start()
{
	return false;
}

bool Script_Aranobot::Update()
{
	return false;
}

void Script_Aranobot::OnDestroy()
{
}

void Script_Aranobot::Attack()
{
}

void Script_Aranobot::AttackCallback()
{
}

void Script_Aranobot::Dead()
{
}

bool Script_Aranobot::Hit(float damage, SPtr_GameObject instigator)
{
	return false;
}
