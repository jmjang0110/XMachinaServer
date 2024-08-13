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

bool Script_Ursacetus::Start()
{
    if (!Script_Enemy::Start()) {
        return false;
    }
 
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Ursacetus::BasicAttackCallback, this), 57);
    GetOwner()->GetAnimation()->GetController()->FindMotionByName(GetStat_Attack3AnimName())->AddCallback(std::bind(&Script_Ursacetus::SpecialAttackCallback, this),65);

    return true;
}

void Script_Ursacetus::BasicAttackCallback()
{
    if (!Script_Enemy::mEnemyController->GetTarget()) {
        return;
    }

    //const float dis = Vec3::Distance(mEnemyMgr->mTarget->GetPosition(), mObject->GetPosition());
    //if (mArmCollider->Intersects(mEnemyMgr->mTarget->GetComponent<Collider>()) || dis < 3.f) {
    //    auto liveObject = mEnemyMgr->mTarget->GetComponent<Script_LiveObject>();
    //    if (liveObject) {
    //        liveObject->Hit(mEnemyMgr->mStat.AttackRate, mObject);
    //    }
    //}
}

void Script_Ursacetus::SpecialAttackCallback()
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

void Script_Ursacetus::AttackEndCallback()
{
    ++mCurrAttackStep;
    mCurrAttackStep %= UrsacetusAttackType::_count;

    GetOwner()->GetAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
}