#include "pch.h"
#include "Script_Gobbler.h"

Script_Gobbler::Script_Gobbler()
{
}

Script_Gobbler::Script_Gobbler(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(5);
    Script_EnemyStat::SetStat_PheroLevel(5);
    Script_EnemyStat::SetStat_MoveSpeed(3);
    Script_EnemyStat::SetStat_DetectionRange(25);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(100);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(500);
    Script_EnemyStat::SetStat_Attack1AnimName("RamAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Roar");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");

    owner->SetName("Gobbler");


}
Script_Gobbler::~Script_Gobbler()
{
}

bool Script_Gobbler::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Gobbler::AttackCallback, this), 20);

    return true;
}