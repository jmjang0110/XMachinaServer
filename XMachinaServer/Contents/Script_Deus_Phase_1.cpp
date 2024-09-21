#include "pch.h"
#include "Script_Deus_Phase_1.h"

/* Script */
#include "Script_EnemyController.h"
#include "Script_Player.h"
#include "Script_BehaviorTrees.h"
#include "Script_BulletDeus_Phase_1.h"
#include "Script_WeaponDeus_Phase_1_Rifle.h"

/* Game Object */
#include "FBsPacketFactory.h"
#include "RoomManager.h"
#include "GameObject.h"
#include "GameRoom.h"
#include "PlayerController.h"

/* Component */
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"



Script_Deus_Phase_1::Script_Deus_Phase_1(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Deus_Phase_1";
    mType = FBProtocol::MONSTER_TYPE_DEUS_PHASE_1;
    owner->SetName(name);
#ifdef SET_DATA_FROM_DATABASE
    Script_EnemyStat::SetDataFromDataBase(name);
#else
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
#endif


    Script_EnemyStat::SetID(owner->GetID());

}

SPtr<Component> Script_Deus_Phase_1::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_AdvancedCombatDroid_5
    auto clonedScript = std::dynamic_pointer_cast<Script_Deus_Phase_1>(target);
    if (clonedScript)
    {
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Deus_Phase_1" << std::endl;
        return nullptr;
    }
}

void Script_Deus_Phase_1::Clone(SPtr<GameObject> target)
{
    // Add a new Script_AdvancedCombatDroid_5 instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Deus_Phase_1>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Deus_Phase_1>(clonedScript));

    clonedScript->SetOwner(target);
}
void Script_Deus_Phase_1::Start()
{
    Script_Enemy::Start();

    // H_Lock ( ±âº» ±ÇÃÑ ) 
    mWeapon = MEMORY->Make_Shared<GameObject>(FBProtocol::ITEM_TYPE_WEAPON_H_LOOK);
    mWeapon->AddComponent<Transform>(Component::Type::Transform);
    auto weapon_entity = mWeapon->SetScriptEntity<Script_WeaponDeus_Phase_1_Rifle>();
    mWeapon->SetOwnerRoom(mOwner->GetOwnerRoom());

    weapon_entity->SetOwnerPlayer(mOwner);
    mWeapon->Start();
}

void Script_Deus_Phase_1::StartAttack()
{
    if (nullptr == mEnemyController->GetTarget()) {
        return;
    }

    const Vec3 myPos = OwnerTransform()->GetPosition();
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
    OwnerAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);

    auto spkt = FBS_FACTORY->SPkt_Monster_State(GetID(), FBProtocol::MONSTER_BT_TYPE_ATTACK, mCurrAttackStep);
    ROOM_MGR->BroadcastRoom(mOwner->GetOwnerRoom()->GetID(), spkt);
}

void Script_Deus_Phase_1::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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

void Script_Deus_Phase_1::MeleeAttack()
{
    mCurrAttackStep = ScriptDeusPhase1AttackType::MeleeAttack;
}

void Script_Deus_Phase_1::RangeAttack()
{
    const float randValue = Math::RandF(0.f, 100.f);
    if (randValue >= mkExplodeAttackRate) {
        Vec3 myPos = GetOwner()->GetTransform()->GetPosition() + Vec3{ 0.f, 3.f, 0.f };
        Vec3 targetPos = mEnemyController->GetTarget()->GetTransform()->GetSnapShot().GetPosition();
        Vec3 dir = Vector3::Normalized(targetPos - myPos);

        mCurrAttackStep = ScriptDeusPhase1AttackType::RangeAttack;
        auto weapon_entity = mWeapon->GetScriptEntity<Script_WeaponDeus_Phase_1_Rifle>();
        if (weapon_entity) {
            weapon_entity->OnShoot(myPos, dir);
            LOG_MGR->Cout("³ª½ô¤»\n");
        }
        
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
