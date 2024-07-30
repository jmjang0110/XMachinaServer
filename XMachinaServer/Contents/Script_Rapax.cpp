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
