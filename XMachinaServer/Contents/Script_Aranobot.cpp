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
    Script_EnemyStat::SetStat_Attack1AnimName("GetHitFront");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
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
    Script_Enemy::Activate();

}

void Script_Aranobot::DeActivate()
{
    Script_Enemy::DeActivate();

}

bool Script_Aranobot::WakeUp()
{
    Script_Enemy::WakeUp();

	return false;
}

bool Script_Aranobot::Start()
{
    Script_Enemy::Start();
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(Script_EnemyStat::GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Aranobot::AttackCallback, this), 6);
	return false;
}

bool Script_Aranobot::Update()
{
    Script_Enemy::Update();

	return false;
}

void Script_Aranobot::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Aranobot::Attack()
{
    Script_Enemy::Attack();

}

void Script_Aranobot::AttackCallback()
{
    Script_Enemy::AttackCallback();

}

void Script_Aranobot::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Aranobot::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

	return false;
}
