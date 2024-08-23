#include "pch.h"
#include "Script_Gobbler.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script_BehaviorTrees.h"

Script_Gobbler::Script_Gobbler()
{
    mType = FBProtocol::MONSTER_TYPE_GOBBLER;

}

Script_Gobbler::Script_Gobbler(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    mType = FBProtocol::MONSTER_TYPE_GOBBLER;

    Script_EnemyStat::SetID(owner->GetID());

    Script_EnemyStat::SetStat_EnemyLevel(5);
    Script_EnemyStat::SetStat_PheroLevel(5);
    Script_EnemyStat::SetStat_MoveSpeed(3);
    Script_EnemyStat::SetStat_DetectionRange(25);
    Script_EnemyStat::SetStat_RotationSpeed(200);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(100);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(500);
    Script_EnemyStat::SetStat_Attack1AnimName("RamAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Roar");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");

    owner->SetName("Gobbler");
}
Script_Gobbler::~Script_Gobbler()
{
}

SPtr<Component> Script_Gobbler::Clone(SPtr<Component> target)
{
    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Gobbler>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Anglerox" << std::endl;
        return nullptr;
    }
}

void Script_Gobbler::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Gobbler>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Gobbler>(clonedScript));

    clonedScript->SetOwner(target);
}
void Script_Gobbler::Start()
{
    Script_Enemy::Start();

    auto AnimController = OwnerAnimation()->GetController();
    AnimController->FindMotionByName(mAttack1AnimName)->AddCallback(std::bind(&Script_Gobbler::AttackCallback, this), 20);

}