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
    Script_EnemyStat::SetMaxHP(200);
    Script_EnemyStat::SetStat_Attack1AnimName("DoubleClawsAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_GetHitName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");


    owner->SetName("Ceratoferox");

}

Script_Ceratoferox::~Script_Ceratoferox()
{
}

bool Script_Ceratoferox::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Ceratoferox::AttackCallback, this), 34);

    return true;
}