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
    Script_EnemyStat::SetDataFromDataBase(name);
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

}

void Script_Deus_Phase_2::StartAttack()
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

void Script_Deus_Phase_2::MeleeAttack()
{
    mCurrAttackStep = ScriptDeusPhase2AttackType::MeleeAttack;
}

void Script_Deus_Phase_2::RangeAttack()
{
    const float randValue = Math::RandF(0.f, 100.f);
    if (randValue >= mkExplodeAttackRate) {
        mCurrAttackStep = ScriptDeusPhase2AttackType::RangeAttack;
    }
    else {
        mCurrAttackStep = ScriptDeusPhase2AttackType::ExplodeAttack;
    }
}
