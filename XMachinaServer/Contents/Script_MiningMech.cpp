#include "pch.h"
#include "Script_MiningMech.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"

#include "Script_BehaviorTrees.h"


Script_MiningMech::Script_MiningMech()
{
    mType = FBProtocol::MONSTER_TYPE_MININGMECH;

}

Script_MiningMech::Script_MiningMech(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    mType = FBProtocol::MONSTER_TYPE_MININGMECH;

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

Script_MiningMech::~Script_MiningMech()
{
}

SPtr<Component> Script_MiningMech::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_MiningMech>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_MiningMech" << std::endl;
        return nullptr;
    }
}

void Script_MiningMech::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_MiningMech>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_MiningMech>(clonedScript));

    clonedScript->SetOwner(target);
}

void Script_MiningMech::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_MiningMech::AttackCallback, this), 15);
    AnimController->FindMotionByName(mAttack2AnimName)->AddCallback(std::bind(&Script_MiningMech::AttackCallback, this), 15);
    AnimController->FindMotionByName(mAttack3AnimName)->AddCallback(std::bind(&Script_MiningMech::SmashAttackCallback, this), 20);

}

void Script_MiningMech::SmashAttackCallback()
{
}

void Script_MiningMech::AttackEndCallback()
{
    ++mCurrAttackStep;
    mCurrAttackStep %= MiningMechAttackType::_count;

    OwnerAnimation()->GetController()->SetValue("Attack", mCurrAttackStep);
    mEnemyController->SetMonsterCurrBTType(FBProtocol::MONSTER_BT_TYPE_IDLE);
}

