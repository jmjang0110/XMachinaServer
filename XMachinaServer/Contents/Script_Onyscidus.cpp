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

bool Script_Onyscidus::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Onyscidus::AttackCallback, this), 17);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Onyscidus::AttackCallback, this), 44);

    return true;
}

void Script_Onyscidus::Clone(SPtr<Component> other)
{
	Script_Enemy::Clone(other);
	SPtr<Script_Onyscidus> otherScript = std::static_pointer_cast<Script_Onyscidus>(other);

}

