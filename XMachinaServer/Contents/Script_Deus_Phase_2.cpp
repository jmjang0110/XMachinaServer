#include "pch.h"
#include "Script_Deus_Phase_2.h"

#include "Script_EnemyController.h"
#include "Script_Player.h"
#include "PlayerController.h"

#include "FBsPacketFactory.h"
#include "GameRoom.h"
#include "RoomManager.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_BehaviorTrees.h"

Script_Deus_Phase_2::Script_Deus_Phase_2(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Deus_Phase_2";
    mType = FBProtocol::MONSTER_TYPE_DEUS_PHASE_2;
    owner->SetName(name);
#ifdef SET_DATA_FROM_DATABASE
    Script_EnemyStat::SetDataFromDataBase(name);
#else
    Script_EnemyStat::SetStat_EnemyLevel(7);
    Script_EnemyStat::SetStat_PheroLevel(6);
    Script_EnemyStat::SetStat_MoveSpeed(7);
    Script_EnemyStat::SetStat_DetectionRange(20);
    Script_EnemyStat::SetStat_RotationSpeed(150);
    Script_EnemyStat::SetStat_AttackRotationSpeed(100);
    Script_EnemyStat::SetStat_AttackRate(300);
    Script_EnemyStat::SetStat_AttackRange(3);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(1500);
    Script_EnemyStat::SetStat_Attack1AnimName("Attack_02");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_GetHitName("Get_Hit_02");
    Script_EnemyStat::SetStat_DeathAnimName("Dead_02");
#endif

    Script_EnemyStat::SetID(owner->GetID());
}

SPtr<Component> Script_Deus_Phase_2::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_AdvancedCombatDroid_5
    auto clonedScript = std::dynamic_pointer_cast<Script_Deus_Phase_2>(target);
    if (clonedScript)
    {
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Deus_Phase_2" << std::endl;
        return nullptr;
    }
}

void Script_Deus_Phase_2::Clone(SPtr<GameObject> target)
{
    // Add a new Script_AdvancedCombatDroid_5 instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Deus_Phase_2>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Deus_Phase_2>(clonedScript));

    clonedScript->SetOwner(target);
}
void Script_Deus_Phase_2::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();

    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Deus_Phase_2::AttackCallback, this), 31);
}

void Script_Deus_Phase_2::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
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
