#include "pch.h"
#include "Script_Deus_Phase_1.h"

#include "Script_EnemyController.h"
#include "Script_Player.h"
#include "PlayerController.h"

#include "FBsPacketFactory.h"
#include "GameRoom.h"
#include "GameManager.h"

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
    Script_EnemyStat::SetStat_AttackRange(10);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(1500);
    Script_EnemyStat::SetStat_Attack1AnimName("Attack_01");
    Script_EnemyStat::SetStat_Attack2AnimName("Shot_01");
    Script_EnemyStat::SetStat_Attack3AnimName("Shot_02");
    Script_EnemyStat::SetStat_GetHitName("Get_Hit");
    Script_EnemyStat::SetStat_DeathAnimName("Dead_02");
    owner->SetName("Deus_Phase_1");
}

bool Script_Deus_Phase_1::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }

    return true;
}

void Script_Deus_Phase_1::StartAttack()
{
    if (nullptr == mEnemyController->GetTarget()) {
        return;
    }

    const Vec3 myPos = GetOwner()->GetTransform()->GetPosition();
    const Vec3 targetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();
    float distance = Vec3::Distance(myPos, targetPos);

    if (distance <= mkMeleeAttackRange) {
        MeleeAttack();
    }
    else {
        RangeAttack();
    }

    mEnemyController->RemoveAllAnimation();
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK);
    GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);
    std::cout << mCurrAttackStep << std::endl;

    auto spkt = FBS_FACTORY->SPkt_Monster_State(GetOwner()->GetID(), FBProtocol::MONSTER_BT_TYPE_ATTACK, mCurrAttackStep);
    GAME_MGR->BroadcastRoom(mEnemyController->GetOwnerRoom()->GetID(), spkt);
}

void Script_Deus_Phase_1::MeleeAttack()
{
    mCurrAttackStep = ScriptDeusPhase1AttackType::MeleeAttack;
}

void Script_Deus_Phase_1::RangeAttack()
{
   const float randValue = Math::RandF(0.f, 100.f);
   if (randValue >= mkExplodeAttackRate) {
       mCurrAttackStep = ScriptDeusPhase1AttackType::RangeAttack;
   }
   else {
       mCurrAttackStep = ScriptDeusPhase1AttackType::ExplodeAttack;
   }
}

//void Script_Deus_Phase_1::SmashAttackCallback()
//{
//}
//
//void Script_Deus_Phase_1::AttackEndCallback()
//{
//    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
//}
//
