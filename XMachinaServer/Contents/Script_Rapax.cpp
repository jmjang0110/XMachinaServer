#include "pch.h"
#include "Script_Rapax.h"
#include "GameObject.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"

#include "Script_BehaviorTrees.h"
Script_Rapax::Script_Rapax()
{
    mType = FBProtocol::MONSTER_TYPE_RAPAX;

}

Script_Rapax::Script_Rapax(SPtr<GameObject> owner)
    :Script_Enemy(owner)
{
    std::string name = "Rapax";
    mType = FBProtocol::MONSTER_TYPE_RAPAX;
    owner->SetName(name);
#ifdef SET_DATA_FROM_DATABASE
    Script_EnemyStat::SetDataFromDataBase(name);
#else
    Script_EnemyStat::SetStat_EnemyLevel(6);
    Script_EnemyStat::SetStat_PheroLevel(6);
    Script_EnemyStat::SetStat_MoveSpeed(4.5);
    Script_EnemyStat::SetStat_DetectionRange(30);
    Script_EnemyStat::SetStat_RotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRotationSpeed(250);
    Script_EnemyStat::SetStat_AttackRate(200);
    Script_EnemyStat::SetStat_AttackRange(2);
    Script_EnemyStat::SetStat_AttackCoolTime(0);
    Script_EnemyStat::SetMaxHP(300);
    Script_EnemyStat::SetStat_Attack1AnimName("JumpClawsAttack");
    Script_EnemyStat::SetStat_Attack2AnimName("None");
    Script_EnemyStat::SetStat_Attack3AnimName("None");
    Script_EnemyStat::SetStat_DeathAnimName("Death");
    Script_EnemyStat::SetStat_GetHitName("GetHitFront");
#endif
    Script_EnemyStat::SetID(owner->GetID());
}

Script_Rapax::~Script_Rapax()
{
}

SPtr<Component> Script_Rapax::Clone(SPtr<Component> target)
{
    Script_Enemy::Clone(target);

    // Try to cast the target to Script_Anglerox
    auto clonedScript = std::dynamic_pointer_cast<Script_Rapax>(target);
    if (clonedScript)
    {
        // Call the base class Clone method first
        Script_Enemy::Clone(clonedScript);
        return clonedScript;
    }
    else
    {
        std::cout << "Clone failed: target is not of type Script_Rapax" << std::endl;
        return nullptr;
    }
}

void Script_Rapax::Clone(SPtr<GameObject> target)
{
    // Add a new Script_Anglerox instance to the GameObject
    auto clonedScript = target->SetScriptEntity<Script_Rapax>();
    // Clone the current script into the new script
    this->Clone(std::dynamic_pointer_cast<Script_Rapax>(clonedScript));
    
    clonedScript->SetOwner(target);

}

void Script_Rapax::Start()
{
    Script_Enemy::Start();

    OwnerAnimation()->GetController()->FindMotionByName(GetStat_Attack1AnimName())->AddCallback(std::bind(&Script_Rapax::AttackCallback, this), 35);

}

void Script_Rapax::Dispatch(OverlappedObject* overlapped, UINT32 bytes)
{
    MEMORY->Delete(overlapped);

}
