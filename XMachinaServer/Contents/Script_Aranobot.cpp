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

bool Script_Aranobot::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Aranobot::AttackCallback, this), 6);

    return true;
}