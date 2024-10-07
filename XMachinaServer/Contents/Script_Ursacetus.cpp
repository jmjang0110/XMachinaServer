#include "pch.h"
#include "Script_Ursacetus.h"
#include "GameObject.h"
#include "Script_EnemyController.h"
#include "Animation.h"
#include "GameRoom.h"

#include "NPCController.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script_BehaviorTrees.h"


Script_Ursacetus::Script_Ursacetus()
    : Script_Enemy()

{
    mType = FBProtocol::MONSTER_TYPE_URSACETUS;

}

Script_Ursacetus::Script_Ursacetus(SPtr<GameObject> owner)
    : Script_Enemy(owner)
{
    std::string name = "Ursacetus";
    mType = FBProtocol::MONSTER_TYPE_URSACETUS;
    owner->SetName(name);
#ifdef SET_DATA_FROM_DATABASE
    Script_EnemyStat::SetDataFromDataBase(name);
    mMaxHP *= 5.f;

#else
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
#endif
    Script_EnemyStat::SetID(owner->GetID());
}

Script_Ursacetus::~Script_Ursacetus()
{
}

SPtr<Component> Script_Ursacetus::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Ursacetus>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Ursacetus" << std::endl;
        return nullptr;
    }
}

void Script_Ursacetus::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Ursacetus>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Ursacetus>(clonedScript));

    clonedScript->SetOwner(target);

}


void Script_Ursacetus::Start()
{
    Script_Enemy::Start();

    auto animController = OwnerAnimation()->GetController();
    animController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Ursacetus::BasicAttackCallback, this), 57);
    animController->FindMotionByName(mAttack3AnimName)->AddCallback(std::bind(&Script_Ursacetus::SpecialAttackCallback, this),65);

}

void Script_Ursacetus::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
    MEMORY->Delete(overlapped);

    int activeReference = mOwner->GetActivateRef();
    if (activeReference > 0) {
        mOwner->Update();
        mOwner->RegisterUpdate();
    }
    else {
        OnExitFromViewList();
    }
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
    //// �Ÿ��� ���� ������
    //const float specialAttackRange = mEnemyMgr->mStat.AttackRange * 2.5f;
    //const float specialAttackRate = mEnemyMgr->mStat.AttackRate * 2.f;
    //const float dis = Vec3::Distance(mEnemyMgr->mTarget->GetPosition(), mObject->GetPosition());
    //if (dis <= specialAttackRange) {
    //    const float damageRatio = 1.f - dis / specialAttackRange;	// �Ÿ��� ���� ������ ����
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

    OwnerAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
}