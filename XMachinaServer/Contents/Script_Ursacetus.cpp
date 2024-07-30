#include "pch.h"
#include "Script_Ursacetus.h"
#include "GameObject.h"
#include "Script_EnemyController.h"
#include "Animation.h"
#include "GameMonster.h"
#include "GameRoom.h"

#include "NPCController.h"


Script_Ursacetus::Script_Ursacetus()
    : Script_Enemy()

{
}

Script_Ursacetus::Script_Ursacetus(SPtr<GameObject> owner, ScriptInfo::Type type)
    : Script_Enemy(owner,  type)
{
    Script_EnemyStat::SetStat_EnemyLevel(4);
    Script_EnemyStat::SetStat_PheroLevel(5);
    Script_EnemyStat::SetStat_MoveSpeed(5);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(150);
    Script_EnemyStat::SetStat_AttackRotationSpeed(10);
    Script_EnemyStat::SetStat_AttackRate(200);
    Script_EnemyStat::SetStat_AttackRange(5);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(1000);
    Script_EnemyStat::SetStat_Attack1AnimName("LeftHandAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("Roar1");
    Script_EnemyStat::SetStat_Attack3AnimName("LeftFootStompAttack");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("None");

    owner->SetName("Ursacetus");
}

Script_Ursacetus::~Script_Ursacetus()
{
}

void Script_Ursacetus::Clone(SPtr<Component> other) 
{
    Script_Enemy::Clone(other);
    SPtr<Script_Ursacetus> otherScript = std::static_pointer_cast<Script_Ursacetus>(other);

}

void Script_Ursacetus::Activate()
{
}

void Script_Ursacetus::DeActivate()
{
}

bool Script_Ursacetus::WakeUp()
{
    Script_Enemy::WakeUp();

    return true;
}

bool Script_Ursacetus::Start()
{
    Script_Enemy::Start();

    return true;
}

bool Script_Ursacetus::Update()
{
    Script_Enemy::Update();

    return true;
}

void Script_Ursacetus::OnDestroy()
{
    Script_Enemy::OnDestroy();

}

void Script_Ursacetus::Attack()
{
    Script_Enemy::Attack();

    bool b{};
    if (mCurrAttackCnt < 2) {
        if (mAnimTime >= 4.33f) {
            b = true;
            std::cout << "ATtack1\n";
            mEnemyController->SetState(EnemyInfo::State::Attack);
            mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_1);
        }
    }
    else if (mCurrAttackCnt < 3) {
        if (mAnimTime >= 5.16f) {
            b = true;
            std::cout << "ATtack2\n";
            mEnemyController->SetState(EnemyInfo::State::Attack2);
            mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_2);
        }
    }
    else if (mCurrAttackCnt < 4) {
        if (mAnimTime >= 3.83f) {
            b = true;
            std::cout << "ATtack3\n";
            mEnemyController->SetState(EnemyInfo::State::Attack3);
            mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_3);
        }
    }

    if (b) {
        mAnimTime = 0.f;
        ++mCurrAttackCnt;
        mCurrAttackCnt %= AttackTypeCount;
    }
}

void Script_Ursacetus::Dead()
{
    Script_Enemy::Dead();

}

bool Script_Ursacetus::Hit(float damage, SPtr_GameObject instigator)
{
    Script_Enemy::Hit(damage, instigator);

    return false;
}

void Script_Ursacetus::BasicStartCallback()
{
}

void Script_Ursacetus::BasicAttackCallback()
{
    if (!Script_Enemy::mEnemyController->GetTarget()) {
        return;
    }

    mEnemyController->SetState(EnemyInfo::State::Attack);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_1);
    //mEnemyController->GetOwnerMonster()->Broadcast_SPkt_Mosnter_State(FBProtocol::MONSTER_BT_TYPE_ATTACK);

    //const float dis = Vec3::Distance(mEnemyMgr->mTarget->GetPosition(), mObject->GetPosition());
    //if (mArmCollider->Intersects(mEnemyMgr->mTarget->GetComponent<Collider>()) || dis < 3.f) {
    //    auto liveObject = mEnemyMgr->mTarget->GetComponent<Script_LiveObject>();
    //    if (liveObject) {
    //        liveObject->Hit(mEnemyMgr->mStat.AttackRate, mObject);
    //    }
    //}
}

void Script_Ursacetus::RoarStartCallback()
{
    mEnemyController->SetState(EnemyInfo::State::Attack2);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_2);
}

void Script_Ursacetus::RoarAttackCallback()
{
   
}

void Script_Ursacetus::SpecialAttack()
{
    //// 거리에 따른 데미지
    //const float specialAttackRange = mEnemyMgr->mStat.AttackRange * 2.5f;
    //const float specialAttackRate = mEnemyMgr->mStat.AttackRate * 2.f;
    //const float dis = Vec3::Distance(mEnemyMgr->mTarget->GetPosition(), mObject->GetPosition());
    //if (dis <= specialAttackRange) {
    //    const float damageRatio = 1.f - dis / specialAttackRange;	// 거리에 따른 데미지 비율
    //    auto liveObject = mEnemyMgr->mTarget->GetComponent<Script_LiveObject>();
    //    if (liveObject) {
    //        liveObject->Hit(specialAttackRate * damageRatio, mObject);
    //    }
    //}
}

void Script_Ursacetus::SpecialAttackCallback()
{
}

void Script_Ursacetus::SpecialAttackStartCallback()
{
    mEnemyController->SetState(EnemyInfo::State::Attack3);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_ATTACK_3);
}

void Script_Ursacetus::AttackEndCallback()
{
    ++mCurrAttackCnt;
    mCurrAttackCnt %= AttackTypeCount;

    GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackCnt);
    GetOwner()->GetAnimation()->GetController()->SetValue("IsAttack", false);
    mEnemyController->SetState(EnemyInfo::State::Idle);
}
