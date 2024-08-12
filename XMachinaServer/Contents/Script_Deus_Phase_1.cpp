#include "pch.h"
#include "Script_Deus_Phase_1.h"

Script_Deus_Phase_1::Script_Deus_Phase_1()
{
}

Script_Deus_Phase_1::Script_Deus_Phase_1(SPtr<GameObject> owner, ScriptInfo::Type type)
    :Script_Enemy(owner, type)
{
    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(7);
    Script_EnemyStat::SetStat_PheroLevel(6);
    Script_EnemyStat::SetStat_MoveSpeed(5.5);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(150);
    Script_EnemyStat::SetStat_AttackRotationSpeed(100);
    Script_EnemyStat::SetStat_AttackRate(300);
    Script_EnemyStat::SetStat_AttackRange(5);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(1500);
    Script_EnemyStat::SetStat_Attack1AnimName("UppercutDiggerAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("UppercutDrillAttack");
    Script_EnemyStat::SetStat_Attack3AnimName("SmashAttack");
    Script_EnemyStat::SetStat_GetHitName("Blocked");
    Script_EnemyStat::SetStat_DeathAnimName("Deactivation");


    owner->SetName("MiningMech");

}

Script_Deus_Phase_1::~Script_Deus_Phase_1()
{
}

bool Script_Deus_Phase_1::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    return true;
}

void Script_Deus_Phase_1::Clone(SPtr<Component> other)
{
    Script_Enemy::Clone(other);
    SPtr<Script_Deus_Phase_1> otherScript = std::static_pointer_cast<Script_Deus_Phase_1>(other);
}

void Script_Deus_Phase_1::SmashAttackCallback()
{
}

void Script_Deus_Phase_1::AttackEndCallback()
{
    ++mCurrAttackStep;
    mCurrAttackStep %= ScriptDeusPhase1AttackType::_count;

    GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
}

